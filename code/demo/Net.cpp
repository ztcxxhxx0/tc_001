#include "Net.h"
#include "ui_Net.h"
#include <iostream>
#include <QMessageBox>

using namespace std;

#include<QFile>
#include<SylixOS.h>

struct ifconfigall icall[5];
int focus_n = 0;

void show_all_msg();
void show_addr_msg ();
void show_mac_msg ();
void show_mask_msg ();
void check_ifparam();

int set_addr(char name[10], char addr[64]);
int set_mask(char name[10], char mask[64]);

char* checkzero(char st[64]);

extern int ping(char* argv);


Net::Net(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Net)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(btnclick()));

    connect(ui->pushButton_save, SIGNAL(clicked()), this, SLOT(btn_save_click()));

    connect(ui->pushButton_shutdown, SIGNAL(clicked()), this, SLOT(btn_shutdown_click()));

    connect(ui->pushButton_edit_addr, SIGNAL(clicked()), this, SLOT(edit_addr_click()));
    connect(ui->pushButton_edit_mask, SIGNAL(clicked()), this, SLOT(edit_mask_click()));
    connect(ui->pushButton_edit_ping, SIGNAL(clicked()), this, SLOT(edit_ping_click()));

    connect(ui->pushButton_ok_addr, SIGNAL(clicked()), this, SLOT(ok_addr_click()));
    connect(ui->pushButton_ok_mask, SIGNAL(clicked()), this, SLOT(ok_mask_click()));
    connect(ui->pushButton_ok_ping, SIGNAL(clicked()), this, SLOT(ok_ping_click()));

    connect(ui->pushButton_ping, SIGNAL(clicked()), this, SLOT(ping_click()));

    connect(ui->pushButton_en1, SIGNAL(clicked()), this, SLOT(en_1_click()));
    connect(ui->pushButton_en2, SIGNAL(clicked()), this, SLOT(en_2_click()));
    connect(ui->pushButton_en3, SIGNAL(clicked()), this, SLOT(en_3_click()));
    connect(ui->pushButton_en4, SIGNAL(clicked()), this, SLOT(en_4_click()));
    connect(ui->pushButton_en5, SIGNAL(clicked()), this, SLOT(en_5_click()));

    connect(ui->pushButton_num_0, SIGNAL(clicked()), this, SLOT(num_0_click()));
    connect(ui->pushButton_num_1, SIGNAL(clicked()), this, SLOT(num_1_click()));
    connect(ui->pushButton_num_2, SIGNAL(clicked()), this, SLOT(num_2_click()));
    connect(ui->pushButton_num_3, SIGNAL(clicked()), this, SLOT(num_3_click()));
    connect(ui->pushButton_num_4, SIGNAL(clicked()), this, SLOT(num_4_click()));
    connect(ui->pushButton_num_5, SIGNAL(clicked()), this, SLOT(num_5_click()));
    connect(ui->pushButton_num_6, SIGNAL(clicked()), this, SLOT(num_6_click()));
    connect(ui->pushButton_num_7, SIGNAL(clicked()), this, SLOT(num_7_click()));
    connect(ui->pushButton_num_8, SIGNAL(clicked()), this, SLOT(num_8_click()));
    connect(ui->pushButton_num_9, SIGNAL(clicked()), this, SLOT(num_9_click()));
    connect(ui->pushButton_num_dot, SIGNAL(clicked()), this, SLOT(num_dot_click()));
    connect(ui->pushButton_num_back, SIGNAL(clicked()), this, SLOT(num_back_click()));

    ui->pushButton_en1->hide();
    ui->pushButton_en2->hide();
    ui->pushButton_en3->hide();
    ui->pushButton_en4->hide();
    ui->pushButton_en5->hide();

    ui->pushButton_ok_addr->hide();
    ui->pushButton_ok_mask->hide();
    ui->pushButton_ok_ping->hide();

    keyboard_disable();


    setWindowIcon(QIcon(":/new/prefix1/icon.png"));

}

Net::~Net()
{
    delete ui;
}

void Net::btn_1_show(char text[10] )
{
    ui->pushButton_en1->setText(text);
    ui->pushButton_en1->show();
    ui->pushButton_en2->hide();
    ui->pushButton_en3->hide();
    ui->pushButton_en4->hide();
    ui->pushButton_en5->hide();
}
void Net::btn_2_show(char text[10] )
{
    ui->pushButton_en2->setText(text);
    ui->pushButton_en2->show();
    ui->pushButton_en3->hide();
    ui->pushButton_en4->hide();
    ui->pushButton_en5->hide();
}
void Net::btn_3_show(char text[10] )
{
    ui->pushButton_en3->setText(text);
    ui->pushButton_en3->show();
    ui->pushButton_en4->hide();
    ui->pushButton_en5->hide();
}
void Net::btn_4_show(char text[10] )
{
    ui->pushButton_en4->setText(text);
    ui->pushButton_en4->show();
    ui->pushButton_en5->hide();
}
void Net::btn_5_show(char text[10] )
{
    ui->pushButton_en5->setText(text);
    ui->pushButton_en5->show();
}



void Net::btnclick()
{
    struct if_nameindex *if_ni, *p;
    int count = 0;
    char iname[5][10] ;

    if_ni = if_nameindex();
    if (if_ni == NULL) {
        fprintf(stderr, "if_nameindex error.\n");
        char str_out[64] = "网卡目录读取失败.";
        output(str_out);
        return ;
    }

    for (p = if_ni; !(p->if_index == 0 && p->if_name[0] == '\0'); p++) {
//        fprintf(stdout, "%u: %s\n", p->if_index, p->if_name);
        strcpy(iname[count], p->if_name);
        count++;
    }

    if_freenameindex(if_ni);

    if (count > 0)
        btn_1_show(iname[0]);
    if (count > 1)
        btn_2_show(iname[1]);
    if (count > 2)
        btn_3_show(iname[2]);
    if (count > 3)
    {
        printf("More than three net.\n");
        char str_out2[64] = "超过三个网卡.";
        output(str_out2);
    }

    show_all_msg();

    ui->if_name->setText("");
    ui->if_addr->setText("");
    ui->if_mask->setText("");
    ui->if_mac->setText("");
    ui->text_ping->setText("");
    ui->label_output->setText("Hello SylixOS！");
    ui->text_output->setText("");

    ui->pushButton_edit_addr->setEnabled(true);
    ui->pushButton_edit_mask->setEnabled(true);

    ui->pushButton_ping->setEnabled(false);

    ui->pushButton_save->setEnabled(false);

    ui->pushButton->setText("刷新");

}



void Net::en_1_click()
{
    ui->if_name->setText(icall[0].name);
    ui->if_addr->setText(icall[0].ip);
    ui->if_mask->setText(icall[0].netmask);
    ui->if_mac->setText(icall[0].mac);

    ui->pushButton_save->setEnabled(false);
}

void Net::en_2_click()
{
    ui->if_name->setText(icall[1].name);
    ui->if_addr->setText(icall[1].ip);
    ui->if_mask->setText(icall[1].netmask);
    ui->if_mac->setText(icall[1].mac);

    ui->pushButton_save->setEnabled(false);
}

void Net::en_3_click()
{
    ui->if_name->setText(icall[2].name);
    ui->if_addr->setText(icall[2].ip);
    ui->if_mask->setText(icall[2].netmask);
    ui->if_mac->setText(icall[2].mac);

    ui->pushButton_save->setEnabled(false);
}

void Net::en_4_click()
{
    ui->if_name->setText(icall[3].name);
    ui->if_addr->setText(icall[3].ip);
    ui->if_mask->setText(icall[3].netmask);
    ui->if_mac->setText(icall[3].mac);

    ui->pushButton_save->setEnabled(false);
}

void Net::en_5_click()
{
    ui->if_name->setText(icall[4].name);
    ui->if_addr->setText(icall[4].ip);
    ui->if_mask->setText(icall[4].netmask);
    ui->if_mac->setText(icall[4].mac);

    ui->pushButton_save->setEnabled(false);
}




void Net::keyboard_enable()
{
    ui->pushButton_num_0->setEnabled(true);
    ui->pushButton_num_1->setEnabled(true);
    ui->pushButton_num_2->setEnabled(true);
    ui->pushButton_num_3->setEnabled(true);
    ui->pushButton_num_4->setEnabled(true);
    ui->pushButton_num_5->setEnabled(true);
    ui->pushButton_num_6->setEnabled(true);
    ui->pushButton_num_7->setEnabled(true);
    ui->pushButton_num_8->setEnabled(true);
    ui->pushButton_num_9->setEnabled(true);
    ui->pushButton_num_dot->setEnabled(true);
    ui->pushButton_num_back->setEnabled(true);
}

void Net::keyboard_disable()
{
    ui->pushButton_num_0->setEnabled(false);
    ui->pushButton_num_1->setEnabled(false);
    ui->pushButton_num_2->setEnabled(false);
    ui->pushButton_num_3->setEnabled(false);
    ui->pushButton_num_4->setEnabled(false);
    ui->pushButton_num_5->setEnabled(false);
    ui->pushButton_num_6->setEnabled(false);
    ui->pushButton_num_7->setEnabled(false);
    ui->pushButton_num_8->setEnabled(false);
    ui->pushButton_num_9->setEnabled(false);
    ui->pushButton_num_dot->setEnabled(false);
    ui->pushButton_num_back->setEnabled(false);
}

void Net::num_0_click()
{
    if (focus_n == 1)
    {
        QString str = ui->if_addr->text();
        str.append("0");
        ui->if_addr->setText(str);
    }
    else if (focus_n == 2)
    {
        QString str = ui->if_mask->text();
        str.append("0");
        ui->if_mask->setText(str);
    }
    else if (focus_n == 3)
    {
        QString str = ui->text_ping->text();
        str.append("0");
        ui->text_ping->setText(str);
    }
    else
    {
        printf("add 0 error!\n");
        return ;
    }
}

void Net::num_1_click()
{

    if (focus_n == 1)
    {
        QString str = ui->if_addr->text();
        str.append("1");
        ui->if_addr->setText(str);
    }
    else if (focus_n == 2)
    {
        QString str = ui->if_mask->text();
        str.append("1");
        ui->if_mask->setText(str);
    }
    else if (focus_n == 3)
    {
        QString str = ui->text_ping->text();
        str.append("1");
        ui->text_ping->setText(str);
    }
    else
    {
        printf("add 1 error!\n");
        return ;
    }
}

void Net::num_2_click()
{
    if (focus_n == 1)
    {
        QString str = ui->if_addr->text();
        str.append("2");
        ui->if_addr->setText(str);
    }
    else if (focus_n == 2)
    {
        QString str = ui->if_mask->text();
        str.append("2");
        ui->if_mask->setText(str);
    }
    else if (focus_n == 3)
    {
        QString str = ui->text_ping->text();
        str.append("2");
        ui->text_ping->setText(str);
    }
    else
    {
        printf("add 2 error!\n");
        return ;
    }
}

void Net::num_3_click()
{
    if (focus_n == 1)
    {
        QString str = ui->if_addr->text();
        str.append("3");
        ui->if_addr->setText(str);
    }
    else if (focus_n == 2)
    {
        QString str = ui->if_mask->text();
        str.append("3");
        ui->if_mask->setText(str);
    }
    else if (focus_n == 3)
    {
        QString str = ui->text_ping->text();
        str.append("3");
        ui->text_ping->setText(str);
    }
    else
    {
        printf("add 3 error!\n");
        return ;
    }
}

void Net::num_4_click()
{
    if (focus_n == 1)
    {
        QString str = ui->if_addr->text();
        str.append("4");
        ui->if_addr->setText(str);
    }
    else if (focus_n == 2)
    {
        QString str = ui->if_mask->text();
        str.append("4");
        ui->if_mask->setText(str);
    }
    else if (focus_n == 3)
    {
        QString str = ui->text_ping->text();
        str.append("4");
        ui->text_ping->setText(str);
    }
    else
    {
        printf("add 4 error!\n");
        return ;
    }
}

void Net::num_5_click()
{
    if (focus_n == 1)
    {
        QString str = ui->if_addr->text();
        str.append("5");
        ui->if_addr->setText(str);
    }
    else if (focus_n == 2)
    {
        QString str = ui->if_mask->text();
        str.append("5");
        ui->if_mask->setText(str);
    }
    else if (focus_n == 3)
    {
        QString str = ui->text_ping->text();
        str.append("5");
        ui->text_ping->setText(str);
    }
    else
    {
        printf("add 5 error!\n");
        return ;
    }
}

void Net::num_6_click()
{
    if (focus_n == 1)
    {
        QString str = ui->if_addr->text();
        str.append("6");
        ui->if_addr->setText(str);
    }
    else if (focus_n == 2)
    {
        QString str = ui->if_mask->text();
        str.append("6");
        ui->if_mask->setText(str);
    }
    else if (focus_n == 3)
    {
        QString str = ui->text_ping->text();
        str.append("6");
        ui->text_ping->setText(str);
    }
    else
    {
        printf("add 6 error!\n");
        return ;
    }
}

void Net::num_7_click()
{
    if (focus_n == 1)
    {
        QString str = ui->if_addr->text();
        str.append("7");
        ui->if_addr->setText(str);
    }
    else if (focus_n == 2)
    {
        QString str = ui->if_mask->text();
        str.append("7");
        ui->if_mask->setText(str);
    }
    else if (focus_n == 3)
    {
        QString str = ui->text_ping->text();
        str.append("7");
        ui->text_ping->setText(str);
    }
    else
    {
        printf("add 7 error!\n");
        return ;
    }
}

void Net::num_8_click()
{
    if (focus_n == 1)
    {
        QString str = ui->if_addr->text();
        str.append("8");
        ui->if_addr->setText(str);
    }
    else if (focus_n == 2)
    {
        QString str = ui->if_mask->text();
        str.append("8");
        ui->if_mask->setText(str);
    }
    else if (focus_n == 3)
    {
        QString str = ui->text_ping->text();
        str.append("8");
        ui->text_ping->setText(str);
    }
    else
    {
        printf("add 8 error!\n");
        return ;
    }
}

void Net::num_9_click()
{
    if (focus_n == 1)
    {
        QString str = ui->if_addr->text();
        str.append("9");
        ui->if_addr->setText(str);
    }
    else if (focus_n == 2)
    {
        QString str = ui->if_mask->text();
        str.append("9");
        ui->if_mask->setText(str);
    }
    else if (focus_n == 3)
    {
        QString str = ui->text_ping->text();
        str.append("9");
        ui->text_ping->setText(str);
    }
    else
    {
        printf("add 9 error!\n");
        return ;
    }
}

void Net::num_dot_click()
{
    if (focus_n == 1)
    {
        QString str = ui->if_addr->text();
        str.append(".");
        ui->if_addr->setText(str);
    }
    else if (focus_n == 2)
    {
        QString str = ui->if_mask->text();
        str.append(".");
        ui->if_mask->setText(str);
    }
    else if (focus_n == 3)
    {
        QString str = ui->text_ping->text();
        str.append(".");
        ui->text_ping->setText(str);
    }
    else
    {
        printf("add . error!\n");
        return ;
    }
}

void Net::num_back_click()
{
    char str_back [64] = "";
    if (focus_n == 1)
    {
        std::string dome = ui->if_addr->text().toStdString();
        strcpy(str_back, dome.c_str());
        int len = ui->if_addr->text().length();
        if (len <= 0)
        {
            printf("None!\n");
            return;
        }
        str_back[len-1] = '\0';
        ui->if_addr->setText(str_back);
    }
    else if (focus_n == 2)
    {
        std::string dome = ui->if_mask->text().toStdString();
        strcpy(str_back, dome.c_str());
        int len = ui->if_mask->text().length();
        if (len <= 0)
        {
            printf("None!\n");
            return;
        }
        str_back[len-1] = '\0';
        ui->if_mask->setText(str_back);
    }
    else if (focus_n == 3)
    {
        std::string dome = ui->text_ping->text().toStdString();
        strcpy(str_back, dome.c_str());
        int len = ui->text_ping->text().length();
        if (len <= 0)
        {
            printf("None!\n");
            return;
        }
        str_back[len-1] = '\0';
        ui->text_ping->setText(str_back);
    }
    else
    {
        printf("back error!\n");
        return ;
    }
}





void Net::edit_addr_click()
{
    if (focus_n != 0)
    {
        ok_mask_click();
        ok_ping_click();
    }

    ui->if_addr->setText("");
    focus_n = 1;

    ui->pushButton_edit_addr->hide();
    ui->pushButton_ok_addr->show();

    keyboard_enable();

    ui->pushButton_save->setEnabled(false);
}

void Net::edit_mask_click()
{
    if (focus_n != 0)
    {
        ok_addr_click();
        ok_ping_click();
    }

    ui->if_mask->setText("");
    focus_n = 2;

    ui->pushButton_edit_mask->hide();
    ui->pushButton_ok_mask->show();

    keyboard_enable();

    ui->pushButton_save->setEnabled(false);
}

void Net::edit_ping_click()
{
    if (focus_n != 0)
    {
        ok_addr_click();
        ok_mask_click();
    }

    ui->text_ping->setText("");
    focus_n = 3;

    ui->pushButton_edit_ping->hide();
    ui->pushButton_ok_ping->show();

    keyboard_enable();

    ui->pushButton_ping->setEnabled(false);
}

void Net::ok_addr_click()
{
    focus_n = 0;
    ui->pushButton_ok_addr->hide();
    ui->pushButton_edit_addr->show();

    char str_addr[64] = "";
    std::string dome = ui->if_addr->text().toStdString();
    strcpy(str_addr, dome.c_str());

    ui->if_addr->setText(checkzero(str_addr));

    if(checkip(str_addr) == 1)
    {
        char str_name[10] = "";
        dome = ui->if_name->text().toStdString();
        strcpy(str_name, dome.c_str());

        if(set_addr(str_name, str_addr) == 1)
        {
            char str_out[64] = "IP地址： ";
            strcat(str_out, str_addr);
            strcat(str_out, " 设置成功！");
            output(str_out);

            if(strcmp(str_name,"lo0"))
                ui->pushButton_save->setEnabled(true);
        }
        else
        {
            char str_out2[64] = "IP地址： ";
            strcat(str_out2, str_addr);
            strcat(str_out2, " 设置失败 ： 未选择网卡");
            output(str_out2);
        }
    }
    show_all_msg();

    keyboard_disable();
}

void Net::ok_mask_click()
{
    focus_n = 0;
    ui->pushButton_ok_mask->hide();
    ui->pushButton_edit_mask->show();

    char str_mask[64] = "";
    std::string dome = ui->if_mask->text().toStdString();
    strcpy(str_mask, dome.c_str());

    ui->if_mask->setText(checkzero(str_mask));

    if(checkip(str_mask) == 1)
    {
        char str_name[10] = "";
        dome = ui->if_name->text().toStdString();
        strcpy(str_name, dome.c_str());

        if(set_mask(str_name, str_mask) == 1)
        {
            char str_out[64] = "掩码地址： ";
            strcat(str_out, str_mask);
            strcat(str_out, " 设置成功！");
            output(str_out);

            if(strcmp(str_name,"lo0"))
                ui->pushButton_save->setEnabled(true);
        }
        else
        {
            char str_out2[64] = "掩码地址： ";
            strcat(str_out2, str_mask);
            strcat(str_out2, " 设置失败 ： 未选择网卡");
            output(str_out2);
        }
    }
    show_all_msg();

    keyboard_disable();
}

void Net::ok_ping_click()
{
    focus_n = 0;
    ui->pushButton_ok_ping->hide();
    ui->pushButton_edit_ping->show();

    char str_ping[64] = "";
    std::string dome = ui->text_ping->text().toStdString();
    strcpy(str_ping, dome.c_str());

    ui->text_ping->setText(checkzero(str_ping));

    int k = checkip(str_ping);
    if(k == 1)
    {
        ui->pushButton_ping->setEnabled(true);
    }

    keyboard_disable();
}

void Net::ping_click()
{


    char str_ping[64] = "";
    std::string dome = ui->text_ping->text().toStdString();
    strcpy(str_ping, dome.c_str());

    ping(str_ping);
}





void Net::output(char msg[64])
{
    ui->label_output->setText(msg);

    QString str = QString::fromStdString(msg);

    str.append("\n");
    str.append("\n");
    str.append(ui->text_output->toPlainText());
    ui->text_output->setText(str);
}





int set_addr(char name[10], char addr[64])
{
    struct ifreq ifr;

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        printf("socket error\n");
        return -1;
    }

    strcpy(ifr.ifr_name, name);
    ioctl(fd, SIOCGIFADDR, &ifr);

    ((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr.s_addr = inet_addr(addr);

    if( ioctl(fd, SIOCSIFADDR, &ifr) < 0)
    {
        printf("Set failed!\n");
        return 0;
    }
    else
    {
        printf("Set successful\n");
        return 1;
    }
}

int set_mask(char name[10], char mask[64])
{
    struct ifreq ifr;

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        printf("socket error\n");
        return -1;
    }

    strcpy(ifr.ifr_name, name);
    ioctl(fd, SIOCGIFNETMASK, &ifr);

    ((struct sockaddr_in*)&ifr.ifr_netmask)->sin_addr.s_addr = inet_addr(mask);

    if( ioctl(fd, SIOCSIFNETMASK, &ifr) < 0)
    {
        printf("Set failed!\n");
        return 0;
    }
    else
    {
        printf("Set successful\n");
        return 1;
    }
}




void show_all_msg()
{
    show_addr_msg();
    show_mask_msg();
    show_mac_msg();
}

void show_addr_msg ()
{
    int            fd;
    int            ret;
    char           buf[512];
    struct ifconf  iconf;

    iconf.ifc_len = 512;
    iconf.ifc_buf = (char *)buf;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        fprintf(stderr, "socket error.\n");
        return ;
    }

    ret = ioctl(fd, SIOCGIFCONF, (void *)&iconf);
    if (ret < 0) {
        fprintf(stderr, "ioctl error.\n");
        return ;
    }

    if (iconf.ifc_len >= 512) {
        fprintf(stderr, "ifconf buffer overflow.\n");
        return ;
    }

    struct ifreq* it = iconf.ifc_req;
    const struct ifreq* const end = it + (iconf.ifc_len / sizeof(struct ifreq));

    struct ifreq   ifr1;
    int i = 0;
    for (; it != end; ++it){
        strcpy(ifr1.ifr_name, it->ifr_name);
        if (ioctl(fd, SIOCGIFFLAGS, &ifr1) == 0) {
            if (ioctl(fd, SIOCGIFADDR, &ifr1) == 0) {
//                printf("Interface name : %s , ",ifr1.ifr_name);
                strcpy(icall[i].name, ifr1.ifr_name);
                strcpy(icall[i].ip, inet_ntoa(((struct sockaddr_in*)&ifr1.ifr_addr)->sin_addr));
//                printf("IP: %s\n", icall[i].ip);
                i++;
            }
        }
    }
    close(fd);
}

void show_mac_msg ()
{
    int            fd;
    int            ret;
    char           buf[512];
    struct ifconf  iconf;
    char           szMac[64];

    iconf.ifc_len = 512;
    iconf.ifc_buf = (char *)buf;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        fprintf(stderr, "socket error.\n");
        return ;
    }

    ret = ioctl(fd, SIOCGIFCONF, (void *)&iconf);
    if (ret < 0) {
        fprintf(stderr, "ioctl error.\n");
        return ;
    }

    if (iconf.ifc_len >= 512) {
        fprintf(stderr, "ifconf buffer overflow.\n");
        return ;
    }

    struct ifreq* it = iconf.ifc_req;
    const struct ifreq* const end = it + (iconf.ifc_len / sizeof(struct ifreq));

    int i = 0;
    struct ifreq   ifr2;
    for (; it != end; ++it){
        strcpy(ifr2.ifr_name, it->ifr_name);
        if (ioctl(fd, SIOCGIFFLAGS, &ifr2) == 0) {
            if (ioctl(fd, SIOCGIFHWADDR, &ifr2) == 0) {
                unsigned char * ptr ;
                ptr = (unsigned char  *)&ifr2.ifr_ifru.ifru_hwaddr.sa_data[0];
                snprintf(szMac,64,"%02X:%02X:%02X:%02X:%02X:%02X",*ptr,*(ptr+1),*(ptr+2),*(ptr+3),*(ptr+4),*(ptr+5));
//                printf("Interface name : %s , Mac address : %s \n",ifr2.ifr_name,szMac);
                strcpy(icall[i].mac, szMac);
                i++;
            }
        }
    }
    close(fd);
}

void show_mask_msg ()
{
    int            fd;
    int            ret;
    char           buf[512];
    struct ifconf  iconf;

    iconf.ifc_len = 512;
    iconf.ifc_buf = (char *)buf;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        fprintf(stderr, "socket error.\n");
        return ;
    }

    ret = ioctl(fd, SIOCGIFCONF, (void *)&iconf);
    if (ret < 0) {
        fprintf(stderr, "ioctl error.\n");
        return ;
    }

    if (iconf.ifc_len >= 512) {
        fprintf(stderr, "ifconf buffer overflow.\n");
        return ;
    }

    struct ifreq* it = iconf.ifc_req;
    const struct ifreq* const end = it + (iconf.ifc_len / sizeof(struct ifreq));

    int i = 0;
    struct ifreq   ifr3;
    for (; it != end; ++it){
        strcpy(ifr3.ifr_name, it->ifr_name);
        if (ioctl(fd, SIOCGIFFLAGS, &ifr3) == 0) {
            if (ioctl(fd, SIOCGIFNETMASK, &ifr3) == 0) {
                strcpy(icall[i].netmask, inet_ntoa(((struct sockaddr_in*)&ifr3.ifr_netmask)->sin_addr));
//                printf("netmask: %s\n", icall[i].netmask);
                i++;
            }
        }
    }
    close(fd);
}



char* checkzero(char st[64])
{
    int i =0;
    int j = 0;

    if (st[0] == '\0')
        return st;

    for (i=strlen(st); i>0; i--)
    {
        st[i] = st[i-1];
    }
    st[0] = '.';

    for (i=0; st[i]!='\0'; i++);
    st[i] = '.';
    st[i+1] = '\0';

//    printf("before: %s\n",st);

    for (i=0; st[i]!='\0'; )
    {
        if (st[i] == '0' && st[i-1] == '.' && st[i+1] != '.')
        {
            for(j=i; st[j]!= '\0'; j++)
            {
                st[j] = st[j+1];
            }
        }
        else if (st[i] == '.' && st[i+1] == '.')
        {
            for(j=strlen(st); j > i+1; j--)
            {
                st[j] = st[j-1];
            }
            st[i+1] = '0';
            i++;
        }
        else
            i++;
    }

    for (i=0; st[i]!='\0'; i++)
    {
        st[i] = st[i+1];
    }
    st[i-2] = '\0';
    return st;
}

int Net::checkip(char ip[64])
{
    int i = 0;
    int j = 0;
    int count = 0;
    char ch = ip[0];
    char st[10] = "";
    for (i=0; ip[i]!='\0'; i++)
    {
        ch = ip[i];
        if (ch == '.')
        {
            count++;
            if (count > 3)
            {
                printf("illeage input:too much dot\n");
                char str_out[64] = "";
                strcat(str_out, ip);
                strcat(str_out, "   地址检测不合法：过多的分段！");
                output(str_out);
                return 0;
            }
            st[j] = '\0';
            int t = atoi(st);
            if (t > 255 || t < 0)
            {
                printf("illeage input:large number\n");
                char str_out2[64] = "";
                strcat(str_out2, ip);
                strcat(str_out2, "   地址检测不合法：过大的数据！");
                output(str_out2);
                return 0;
            }
            j = 0;
            continue;
        }
        if (ch < '0' || ch > '9')
        {
            printf("unknown char!\n");
            char str_out3[64] = "";
            strcat(str_out3, ip);
            strcat(str_out3, "   无法识别的字符！");
            output(str_out3);
            return -1;
        }
        st[j]=ch;
        j++;
    }
    st[j] = '\0';

    int t = atoi(st);
    if (t > 255 || t < 0)
    {
        printf("illeage input:large number\n");
        char str_out4[64] = "";
        strcat(str_out4, ip);
        strcat(str_out4, "   地址检测不合法：过大的数据！");
        output(str_out4);
        return 0;
    }

    if (count < 3)
    {
        printf("illeage input:no enough dot\n");
        char str_out5[64] = "";
        strcat(str_out5, ip);
        strcat(str_out5, "   地址检测不合法：分段数不足！");
        output(str_out5);
        return 0;
    }
    char str_out6[64] = "";
    strcat(str_out6, ip);
    strcat(str_out6, "   地址检测合法！");
    output(str_out6);
    return 1;
}

int check_ifparam(char str[64])
{
    if(str[0] == 'i' && str[1] == 'p' && str[2] == 'a' && str[3] == 'd' && str[4] == 'd' && str[5] == 'r')
        return 1;
    else if(str[0] == 'n' && str[1] == 'e' && str[2] == 't' && str[3] == 'm' && str[4] == 'a' && str[5] == 's' && str[6] == 'k')
        return 2;
    else
        return 0;
}

void Net::btn_save_click()
{

    char save_conf[10];
    char save_ip[64];
    char save_mask[64];

    if (QMessageBox::No == QMessageBox::question(this,
                                                  tr("警告"),
                                                  tr("您确定要保存么？"),
                                                  QMessageBox::Yes | QMessageBox::No,
                                                  QMessageBox::Yes))
    {
        return;
    }

    char str_ifparam[100][64];
    FILE *fp;
    fp = fopen("/etc/ifparam.ini", "rt+");
    int count = 0;
    while(fscanf(fp, "%s", str_ifparam[count]) != -1)
    {
        printf("count[%d] : %s\n", count, str_ifparam[count]);
        count++;
    }
    fclose(fp);


    std::string dome = ui->if_name->text().toStdString();
    strcpy(save_conf, dome.c_str());

    dome = ui->if_addr->text().toStdString();
    strcpy(save_ip, dome.c_str());

    dome = ui->if_mask->text().toStdString();
    strcpy(save_mask, dome.c_str());

    int i = 0;
    int flag = 0;
    int flag_ip[3] = {0};
    int flag_mask[3] = {0};
    for(i=0; i<=count; i++)
    {
        if(check_ifparam(str_ifparam[i]) == 1)
            flag_ip[flag++] = i;
    }
    flag = 0;
    for(i=0; i<=count; i++)
    {
        if(check_ifparam(str_ifparam[i]) == 2)
        {
            flag_mask[flag++] = i;
        }
    }

    if( flag_ip == 0 || flag_mask == 0)
    {
        char str[64] = "文件内无有效信息！";
        output(str);
        return;
    }

    if (strcmp(save_conf , "en1") == 0)
    {
        strcpy(str_ifparam[flag_ip[0]], "ipaddr=");
        strcat(str_ifparam[flag_ip[0]], save_ip);
        strcpy(str_ifparam[flag_mask[0]], "netmask=");
        strcat(str_ifparam[flag_mask[0]], save_mask);
    }
    else if (strcmp(save_conf , "en2") == 0)
    {
        strcpy(str_ifparam[flag_ip[1]], "ipaddr=");
        strcat(str_ifparam[flag_ip[1]], save_ip);
        strcpy(str_ifparam[flag_mask[1]], "netmask=");
        strcat(str_ifparam[flag_mask[1]], save_mask);
    }

    int count2 = 0;
    fp = fopen("/etc/ifparam.ini", "wt+");
    for(count2=0; count2<count; count2++)
    {
        fprintf(fp, "%s\n\n", str_ifparam[count2]);
    }
    fclose(fp);

    char str[64] = "保存成功！";
    output(str);

    ui->pushButton_save->setEnabled(false);
}



void Net::btn_shutdown_click()
{
    system("shutdown");
}


