#ifndef STARTUI_H
#define STARTUI_H

#include <QMainWindow>
#include "serial.h"
#include "filemanager.h"
#include <Net.h>
#include <QString>
#include "GPIO.h"

namespace Ui {
class StartUi;
}

class StartUi : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartUi(QWidget *parent = 0);
    ~StartUi();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::StartUi *ui;

  //  Net net;
    GPIO gpio;

    FileViewWidget *fi = new FileViewWidget;
};

#endif // STARTUI_H
