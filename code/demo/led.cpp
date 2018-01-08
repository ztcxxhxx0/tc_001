
#include "led.h"
#include "gpio.h"
#include <QFile>
#include <QString>

//pthread_mutex_t lock;
int cancel_flag=0;

/*
*设置led灯编号，使相应led灯闪烁
*/
void *gpio_out(void *arg){
    int led_num;
    led_num = (int)arg;
    int i;
    int fd;
    printf("led_num:%d\n",led_num);
    fd = gpiofd(led_num,O_WRONLY,GPIO_FLAG_OUT_INIT_LOW);

    if(fd <0){
        printf("failed to open gpio %d\n",led_num);
        return (NULL) ;
    }
    for(i=0;i<10;i++){
      //  pthread_mutex_lock(&lock);
        if(cancel_flag!=0){
            cancel_flag = 0;
            break;
        }
       // pthread_mutex_unlock(&lock);
        gpiofd_write(fd,0);
        usleep(500*1000);
        gpiofd_write(fd,1);
        usleep(500*1000);
    }
    cout<<"finished:"<<led_num<<endl;
    close(fd);
    pthread_exit(NULL);
    return (NULL);

}

/*
*设置相应led和key键编号，使之相对应，按键按下时，led会亮起或灭掉
*/

void *gpio_in(void *arg)
{
    int ret;
    fd_set fdset;
    uint8_t value;
    board_num_in *port;
    port = (board_num_in *)arg;
    cout<<"port.key:"<<port->key<<endl;
    int ledfd,keyfd;
    ledfd = gpiofd(port->led,O_WRONLY,GPIO_FLAG_OUT_INIT_LOW);
    if(ledfd<0){
        printf("failed to ledfd：%d\n",port->led);
        return NULL ;
    }
    keyfd = gpiofd(port->key,O_RDONLY,GPIO_FLAG_TRIG_FALL);
    if(keyfd<0){
        printf("failed to keyfd:%d\n",port->key);
        return (NULL) ;
    }
    int i = 0;
    FD_ZERO(&fdset);
    while(i<10){
        if(cancel_flag!=0){
            cancel_flag = 0;
            break;
        }
        FD_SET(keyfd,&fdset);
        ret = select(keyfd+1,&fdset,NULL,NULL,NULL);
        if(ret ==1){
            gpiofd_read(keyfd,&value);
            //printf("value:%d\n",value);
            gpiofd_read(ledfd,&value);
            value = !value;            
            gpiofd_write(ledfd,value);
            usleep(500*1000);
        }else if(ret <0){
            printf("select error!\n");
            break;
        }
        i++;
    }
    close(ledfd);
    close(keyfd);
    pthread_exit(NULL);
    return (NULL);
}

