#ifndef GPIO_H
#define GPIO_H

#include <QDialog>
#include "led.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QTextEdit>
#include <QDialog>
#include <QTextStream>
//#include <pthread.h>

extern int cancel_flag;
#define DATA_PATH "Data.txt"
/*
 * data文件数据格式
 * TYPE
 * 开发板类型
 * LED
 * led灯端口号
 * KEY
 * -1
 * KEY按键端口号
 * #   表示结束
*/
namespace Ui {
class GPIO;
}

class GPIO : public QDialog
{
    Q_OBJECT

public:
    explicit GPIO(QWidget *parent = 0);
    void Dir_out();
    void Init_combobox_boardType();
    ~GPIO();

private slots:
    void on_run_clicked();

    //void on_pushButton_16_clicked();



  //  void on_boardType_activated(int index);

  /*  void on_OK_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_14_clicked();

    void on_LEDButton_clicked();

    void on_KEYButton_clicked();*/

    void on_boardType_activated(const QString &arg1);

    void on_cancel_clicked();

    void on_pushButton_clicked();

private:
    Ui::GPIO *ui;
};

#endif // GPIO_H
