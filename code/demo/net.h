#ifndef DIALOG
#define DIALOG

#include <QDialog>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
//#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>


struct ifconfigall{
    char name[10];
    char ip[64];
    char netmask[64];
    char gateway[64];
    char mac[64];
};


namespace Ui {
class Net;
}

class Net: public QDialog
{
    Q_OBJECT


public:
    explicit Net(QWidget *parent = 0);
    ~Net();

private slots:
    void btnclick();
    void btn_save_click();
    void btn_shutdown_click();

private slots:
    void btn_1_show(char *);
    void btn_2_show(char *);
    void btn_3_show(char *);
    void btn_4_show(char *);
    void btn_5_show(char *);

private slots:
    void en_1_click();
    void en_2_click();
    void en_3_click();
    void en_4_click();
    void en_5_click();

private slots:
    void num_0_click();
    void num_1_click();
    void num_2_click();
    void num_3_click();
    void num_4_click();
    void num_5_click();
    void num_6_click();
    void num_7_click();
    void num_8_click();
    void num_9_click();
    void num_dot_click();
    void num_back_click();

private slots:
    void edit_addr_click();
    void edit_mask_click();
    void edit_ping_click();

private slots:
    void ok_addr_click();
    void ok_mask_click();
    void ok_ping_click();

private slots:
    void ping_click();


private slots:
    int ping(char *);


public slots:
    void output(char msg[64]);


private:
    int checkip(char st[64]);
    void keyboard_enable();
    void keyboard_disable();




private:
    Ui::Net *ui;
};




#endif // DIALOG

