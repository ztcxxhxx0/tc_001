#include "mythread.h"
#include <QDebug>
#include <QIcon>
MyThread::MyThread(QThread *parent) : QThread(parent)
{
}

void MyThread::run(){
    if (fd<=0) return ;
    char buff[256];
    waitit=0;
    stopit=0;
    while( !stopit){     //1 ){
        if(!waitit){
            int nread=read(fd,buff,255);
            if(nread>0) {
                write(fd,buff,nread);
                memset(buff+nread,'\0',255-nread);
                buff_out=QString::fromLocal8Bit(buff);
                emit stringChanged(buff_out);
                qDebug("emit: %s",buff);
                waitit=1;
            }
        }
    }
}

void MyThread::ok(){
    waitit=0;
}

void MyThread::shutdown(){
    waitit=1;
    stopit=1;
}
