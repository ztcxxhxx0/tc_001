#include "startui.h"
#include "ui_startui.h"


StartUi::StartUi(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartUi)
{
    ui->setupUi(this);

}

StartUi::~StartUi()
{
    delete ui;
}

void StartUi::on_pushButton_clicked()
{
    Serial ser;
    ser.show();
    this->hide();
    ser.exec();
    this->show();
}

void StartUi::on_pushButton_2_clicked()
{


    //fi->showFullScreen();
    fi->show();
    fi->setFixedSize(792,440);
    fi->move(0,0);
    this->hide();
    fi->exec();
    this->show();

}

void StartUi::on_pushButton_3_clicked()
{
    Net net;
    net.show();
    this->hide();
    net.exec();
    this->show();
}

void StartUi::on_pushButton_4_clicked()
{

    gpio.show();
    this->hide();
    gpio.exec();
    this->show();
}
