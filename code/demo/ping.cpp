#include "Net.h"


#define PACKET_SEND_MAX_NUM 64
#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0

typedef struct ping_packet_status
{
    struct timeval begin_time;
    struct timeval end_time;
    int flag;   //发送标志,1为已发送
    int seq;     //包的序列号
}ping_packet_status;


void icmp_sigint();
ping_packet_status ping_packet[PACKET_SEND_MAX_NUM];


int alive;
int rawsock;
int send_count;
int recv_count;
int flag = 0;

char ping_out[64] = "";

pid_t pid;
struct sockaddr_in dest;
struct timeval start_time;
struct timeval end_time;
struct timeval time_interval;


/*******************************      netinet/ip_icmp.h       *********************************/
struct icmp
{
  u_int8_t  icmp_type;    /* type of message, see below */
  u_int8_t  icmp_code;    /* type sub code */
  u_int16_t icmp_cksum;    /* ones complement checksum of struct */
  union
  {
    u_char ih_pptr;        /* ICMP_PARAMPROB */
    struct in_addr ih_gwaddr;    /* gateway address */
    struct ih_idseq        /* echo datagram */
    {
      u_int16_t icd_id;
      u_int16_t icd_seq;
    } ih_idseq;
    u_int32_t ih_void;

    /* ICMP_UNREACH_NEEDFRAG -- Path MTU Discovery (RFC1191) */
    struct ih_pmtu
    {
      u_int16_t ipm_void;
      u_int16_t ipm_nextmtu;
    } ih_pmtu;

    struct ih_rtradv
    {
      u_int8_t irt_num_addrs;
      u_int8_t irt_wpa;
      u_int16_t irt_lifetime;
    } ih_rtradv;
  } icmp_hun;
#define    icmp_pptr    icmp_hun.ih_pptr
#define    icmp_gwaddr    icmp_hun.ih_gwaddr
#define    icmp_id        icmp_hun.ih_idseq.icd_id
#define    icmp_seq    icmp_hun.ih_idseq.icd_seq
#define    icmp_void    icmp_hun.ih_void
#define    icmp_pmvoid    icmp_hun.ih_pmtu.ipm_void
#define    icmp_nextmtu    icmp_hun.ih_pmtu.ipm_nextmtu
#define    icmp_num_addrs    icmp_hun.ih_rtradv.irt_num_addrs
#define    icmp_wpa    icmp_hun.ih_rtradv.irt_wpa
#define    icmp_lifetime    icmp_hun.ih_rtradv.irt_lifetime
  union
  {
    struct
    {
      u_int32_t its_otime;
      u_int32_t its_rtime;
      u_int32_t its_ttime;
    } id_ts;
    struct
    {
      struct ip idi_ip;
      /* options and then 64 bits of data */
    } id_ip;
 //   struct icmp_ra_addr id_radv;
    u_int32_t   id_mask;
    u_int8_t    id_data[1];
  } icmp_dun;
#define    icmp_otime    icmp_dun.id_ts.its_otime
#define    icmp_rtime    icmp_dun.id_ts.its_rtime
#define    icmp_ttime    icmp_dun.id_ts.its_ttime
#define    icmp_ip        icmp_dun.id_ip.idi_ip
#define    icmp_radv    icmp_dun.id_radv
#define    icmp_mask    icmp_dun.id_mask
#define    icmp_data    icmp_dun.id_data
};

/**********************************************************************************************/



/*校验和算法*/
unsigned short cal_chksum(unsigned short *addr,int len)
{       int nleft=len;
        int sum=0;
        unsigned short *w=addr;
        unsigned short answer=0;

        /*把ICMP报头二进制数据以2字节为单位累加起来*/
        while(nleft>1)
        {
            sum+=*w++;
            nleft-=2;
        }
        /*若ICMP报头为奇数个字节，会剩下最后一字节。把最后一个字节视为一个2字节数据的高字节，这个2字节数据的低字节为0，继续累加*/
        if( nleft==1)
        {
            *(unsigned char *)(&answer)=*(unsigned char *)w;
            sum+=answer;
        }
        sum=(sum>>16)+(sum&0xffff);
        sum+=(sum>>16);
        answer=~sum;
        return answer;
}

struct timeval cal_time_offset(struct timeval begin, struct timeval end)
{
    struct timeval ans;
    ans.tv_sec = end.tv_sec - begin.tv_sec;
    ans.tv_usec = end.tv_usec - begin.tv_usec;
    if(ans.tv_usec < 0) //如果接收时间的usec小于发送时间的usec，则向sec域借位
    {
        ans.tv_sec--;
        ans.tv_usec+=1000000;
    }
    return ans;
}

void icmp_pack(struct icmp* icmphdr, int seq, int length)
{
    int i = 0;

    icmphdr->icmp_type = ICMP_ECHO;
    icmphdr->icmp_code = 0;
    icmphdr->icmp_cksum = 0;
    icmphdr->icmp_seq = seq;
    icmphdr->icmp_id = pid & 0xffff;
    for(i=0;i<length;i++)
    {
        icmphdr->icmp_data[i] = i;
    }

    icmphdr->icmp_cksum = cal_chksum((unsigned short*)icmphdr, length);
}

int icmp_unpack(char* buf, int len)
{
    int iphdr_len;
    struct timeval begin_time, recv_time, offset_time;
    int rtt;  //round trip time

    struct ip* ip_hdr = (struct ip *)buf;
    iphdr_len = ip_hdr->ip_hl*4;
    struct icmp* icmp = (struct icmp*)(buf+iphdr_len);
    len-=iphdr_len;  //icmp包长度
    if(len < 8)   //判断长度是否为ICMP包长度
    {
        fprintf(stderr, "Invalid icmp packet.Its length is less than 8\n");
        strcpy(ping_out, "Invalid icmp packet.Its length is less than 8");
        flag++;
        return -1;
    }

    //判断该包是ICMP回送回答包且该包是我们发出去的
    if((icmp->icmp_type == ICMP_ECHOREPLY) && (icmp->icmp_id == (pid & 0xffff)))
    {
        if((icmp->icmp_seq < 0) || (icmp->icmp_seq > PACKET_SEND_MAX_NUM))
        {
            fprintf(stderr, "icmp packet seq is out of range!\n");
            strcpy(ping_out, "icmp packet seq is out of range!");
            flag++;
            return -1;
        }

        ping_packet[icmp->icmp_seq].flag = 0;
        begin_time = ping_packet[icmp->icmp_seq].begin_time;
        gettimeofday(&recv_time, NULL);

        offset_time = cal_time_offset(begin_time, recv_time);
        rtt = offset_time.tv_sec*1000 + offset_time.tv_usec/1000 ; //毫秒为单位

        snprintf(ping_out, 64, "%d byte from %s: icmp_seq=%u ttl=%d rtt=%d ms\n", len, inet_ntoa(ip_hdr->ip_src), icmp->icmp_seq, ip_hdr->ip_ttl, rtt);
        printf("%s\n",ping_out);

    }
    else
    {
        fprintf(stderr, "Invalid ICMP packet! Its id is not matched!\n");
        strcpy(ping_out, "Invalid ICMP packet! Its id is not matched!");
        flag++;
        return -1;
    }
    return 0;
}

void *ping_send(void *)
{
    char send_buf[128];
    memset(send_buf, 0, sizeof(send_buf));
    gettimeofday(&start_time, NULL); //记录第一个ping包发出的时间
    send_count = 0;
    recv_count = 0;
    int count = 0;
    while(alive)
    {
        if (count++ > 0)
        {
            icmp_sigint();
            break;
        }
        int size = 0;
        gettimeofday(&(ping_packet[send_count].begin_time), NULL);
        ping_packet[send_count].flag = 1; //将该标记为设置为该包已发送

        icmp_pack((struct icmp*)send_buf, send_count, 64); //封装icmp包
        size = sendto(rawsock, send_buf, 64, 0, (struct sockaddr*)&dest, sizeof(dest));
        send_count++; //记录发出ping包的数量
        if(size < 0)
        {
            fprintf(stderr, "send icmp packet fail!\n");
            strcpy(ping_out,"send icmp packet fail!");
            flag++;
            continue;
        }

        sleep(1);
    }
    return NULL;
}

void * ping_recv(void*)
{
    struct timeval tv;
    tv.tv_usec = 200;  //设置select函数的超时时间为200us
    tv.tv_sec = 0;
    fd_set read_fd;
    char recv_buf[512];
    memset(recv_buf, 0 ,sizeof(recv_buf));
    while(alive)
    {
        int ret = 0;
        FD_ZERO(&read_fd);
        FD_SET(rawsock, &read_fd);
        ret = select(rawsock+1, &read_fd, NULL, NULL, &tv);
        switch(ret)
        {
            case -1:
                fprintf(stderr,"fail to select!\n");
                break;
            case 0:
                break;
            default:
                {
                    int size = recv(rawsock, recv_buf, sizeof(recv_buf), 0);
                    if(size < 0)
                    {
                        fprintf(stderr,"recv data fail!\n");
                        strcpy(ping_out,"recv data fail!");
                        flag++;
                        continue;
                    }

                    ret = icmp_unpack(recv_buf, size); //对接收的包进行解封
                    if(ret == -1)  //不是属于自己的icmp包，丢弃不处理
                    {
                        continue;
                    }
                    recv_count++; //接收包计数
                    flag++;
                }
                break;
        }
    }
    if(!flag)
    {
        printf("Timr out\n");
        strcpy(ping_out, "Time out");
    }

    return NULL;
}

void icmp_sigint()
{
    alive = 0;
    gettimeofday(&end_time, NULL);
    time_interval = cal_time_offset(start_time, end_time);
}

void ping_stats_show()
{
    long time = time_interval.tv_sec*1000+time_interval.tv_usec/1000;
    printf("%d packets transmitted, %d recieved, %d%c packet loss, time %ldms\n", send_count, recv_count, (send_count-recv_count)*100/send_count, '%', time);
}




/*ping*/
int Net::ping(char* argv)
{
    int size = 128*1024;//128k
    char dest_addr_str[80];
    memset(dest_addr_str, 0, 80);
    unsigned int inaddr = 1;
    struct hostent* host = NULL;

    memset(ping_out, ' ', 64);
    flag = 0;

    pthread_t send_id,recv_id;

    memcpy(dest_addr_str, argv, strlen(argv)+1);

    rawsock = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
    if(rawsock < 0)
    {
        printf("Fail to create socket!\n");
        return -1;
    }

    pid = getpid();

    setsockopt(rawsock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)); //增大接收缓冲区至128K

    bzero(&dest,sizeof(dest));

    dest.sin_family = AF_INET;

    inaddr = inet_addr(argv);
    if(inaddr == INADDR_NONE)   //判断用户输入的是否为IP地址还是域名
    {
        //输入的是域名地址
        host = gethostbyname(argv);
        if(host == NULL)
        {
            printf("Fail to gethostbyname!\n");
            return -1;
        }

        memcpy((char*)&dest.sin_addr, host->h_addr, host->h_length);
    }
    else
    {
        memcpy((char*)&dest.sin_addr, &inaddr, sizeof(inaddr));//输入的是IP地址
    }
    inaddr = dest.sin_addr.s_addr;

    char buf[64];
    snprintf(buf, 64, "PING %s, (%d.%d.%d.%d) 56(84) bytes of data.",dest_addr_str, (inaddr&0x000000ff), (inaddr&0x0000ff00)>>8, (inaddr&0x00ff0000)>>16, (inaddr&0xff000000)>>24);
    printf("%s\n", buf);
//    output(buf);


    alive = 1;


    if(pthread_create(&send_id, NULL, ping_send, NULL))
    {
        printf("Fail to create ping send thread!\n");
        return -1;
    }

    if(pthread_create(&recv_id, NULL, ping_recv, NULL))
    {
        printf("Fail to create ping recv thread!\n");
        return -1;
    }

    pthread_join(send_id, NULL);
    pthread_join(recv_id, NULL);


    output(ping_out);

    strcpy(ping_out, buf);

    output(ping_out);


    ping_stats_show();

    ::close(rawsock);
    return 0;

}
