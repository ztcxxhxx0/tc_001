#ifndef SERIAL_H
#define SERIAL_H

#include <QDialog>
#include "mythread.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <QThread>
#include <iostream>
#include <QDebug>

extern int fd;






namespace Ui {
class Serial;
}

class Serial : public QDialog
{
    Q_OBJECT

public:
    explicit Serial(QWidget *parent = 0);
    ~Serial();

private slots:
    int on_Start_clicked();

    void on_Stop_clicked();

    void changeString(const QString &);

    void on_Scan_clicked();

    void on_Emit_clicked();

    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_0_clicked();

    void on_pushButton_clr_clicked();

    void on_pushButton_del_clicked();

private:
    Ui::Serial *ui;
    MyThread thread[4];
    int cur;
    int set_speed(int);
    int set_parity(int, int, int);
    struct termios opt;

};

#endif
