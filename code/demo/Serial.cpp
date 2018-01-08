#include "Serial.h"
#include "ui_Serial.h"
Serial::Serial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Serial)
{
    ui->setupUi(this);
    connect(&thread[0],SIGNAL(stringChanged(QString)),this,SLOT(changeString(QString)));
    connect(&thread[1],SIGNAL(stringChanged(QString)),this,SLOT(changeString(QString)));
    connect(&thread[2],SIGNAL(stringChanged(QString)),this,SLOT(changeString(QString)));
    connect(&thread[3],SIGNAL(stringChanged(QString)),this,SLOT(changeString(QString)));
    setWindowIcon(QIcon(":/new/prefix1/icon.png"));
    ui->Check->setCurrentIndex(2);
    ui->Baud_Rate->setCurrentIndex(10);
    this->on_Scan_clicked();
    ui->Emit->setEnabled(false);
    ui->Stop->setEnabled(false);
}

Serial::~Serial()
{
    delete ui;
}

void Serial::on_Stop_clicked()
{
    thread[cur].shutdown();
    if(thread[cur].isRunning()){
        thread[cur].quit();
    }
    ui->Start->setEnabled(true);
    ui->Stop->setEnabled(false);
    ui->Emit->setEnabled(false);
    ui->textBrowser->append("已结束！！！");
}

void Serial::changeString(const QString &str){
    //ui->textBrowser->insertPlainText(str);
    //ui->textBrowser->moveCursor(QTextCursor::Start);
    ui->textBrowser->append(str);
    thread[cur].ok();
}




int Serial::on_Start_clicked()
{

   QString Serial=ui->Serial_List->currentText();
   cur=ui->Serial_List->currentIndex();
   QString path="/dev/"+Serial;
   QByteArray ba=path.toLatin1();
   const char* tty=ba.data();
   fd=::open(tty,O_RDWR);
   if(fd<0){
       ui->textBrowser->setText("设备开启失败");
       return -1;
   }
   ui->textBrowser->setText("设备已打开!");

   int baud=ui->Baud_Rate->currentText().toInt();
   int ret;

   ret=set_speed(baud);
   if(ret!=0){
       ::close(fd);
       return (-1);
   }

   int databits=ui->Data_Bits->currentText().toInt();
   int stopbits=ui->Stop_Bits->currentText().toInt();
   int checknum=ui->Check->currentIndex();
   char checkbits;
   switch(checknum){
   case 0:checkbits='o';
       break;
   case 1:checkbits='e';
       break;
   case 2:checkbits='n';
       break;
   default:
       ui->textBrowser->append("未支持的校验方式");
       return (-1);
   }

   ret=set_parity(databits,stopbits,checkbits);
   if(ret!=0){
       ::close(fd);
       return (-1);
   }

   ui->textBrowser->append("设置完毕！");
 //  thread[cur].restart();
   thread[cur].start();
   ui->Start->setEnabled(false);
   ui->Stop->setEnabled(true);
   ui->Emit->setEnabled(true);
   return 0;
}


void Serial::on_Scan_clicked()
{
    ui->Serial_List->clear();
    char buffer[200];
    chdir("/");
    getcwd(buffer,200);
    DIR *dir;
    dir=opendir("/dev");
    getcwd(buffer,200);
    if(!dir){
        ui->textBrowser->setText("检索设备失败!");
        return ;
    }
    struct dirent *entry;
    struct stat statbuf;
    while(1){
        entry=readdir(dir);
        if(entry==NULL){
            break;
        }
        else{
            lstat(entry->d_name,&statbuf);
          //  int t;
            strcpy(buffer,entry->d_name);
            /*
            t=strncmp(buffer,".",4);
            if (t==0){
                continue;
            }
            t=strncmp(buffer,"..",4);
            if (t==0){
                continue;
            }
         */
            strcpy(buffer,entry->d_name);
           int t=strncmp(buffer,"ttyS",4);
            if (t==0){
                ui->Serial_List->addItem(entry->d_name);
            }
            t=strncmp(buffer,"ttyUSB",6);
            if(t==0){
                ui->Serial_List->addItem(entry->d_name);
            }

       //     ui->Serial_List->addItem(entry->d_name);
        }
    }
}

void Serial::on_Emit_clicked()
{
    QString str=ui->lineEdit->text();
    ui->History->append(str);
    int size=str.size();
    char *buffer;
    QByteArray ba;
    ba=str.toLocal8Bit();
    buffer=ba.data();
    write(fd,buffer,size);
    write(fd,"\n",1);
    qDebug("%s",buffer);
    //ui->lineEdit->clear();
}






int Serial::set_speed(int speed){
    int ret;
    struct termios *option = new termios();
    ret=tcgetattr(fd,option);
    if(ret!=0){
        qDebug("获取串口速率失败");
        qDebug("%d",ret);
        return (1);
    }
    tcflush(fd,TCIOFLUSH);
    cfsetispeed(&opt,speed);
    cfsetospeed(&opt,speed);

    ret=tcsetattr(fd,TCSANOW,option);
    if(ret!=0){
        qDebug("设置串口速率失败");
        return (2);
    }

    return (0);
}


int Serial::set_parity(int databits, int stopbits, int parity){
    int ret;
    ret=tcgetattr(fd,&opt);
    if(ret!=0){
        qDebug("串口设置失败");
        return (1);
    }
    opt.c_cflag&=~CSIZE;
    switch(databits){
    case 5:
        opt.c_cflag |=CS5;
        break;
    case 6:
        opt.c_cflag |= CS6;
        break;
    case 7:
        opt.c_cflag |= CS7;
        break;
    case 8:
        opt.c_cflag |= CS8;
        break;
    default:
        qDebug("不支持的数据位数");
        return (2);
    }

    switch(parity){
    case 'n':
    case 'N':
        opt.c_cflag &= ~PARENB;
        opt.c_iflag &= ~INPCK;
        break;
    case 'o':
    case 'O':
        opt.c_cflag |= (PARODD|PARENB);
        opt.c_iflag |= INPCK;
        break;
    case 'e':
    case 'E':
        opt.c_cflag |=PARENB;
        opt.c_cflag &=~PARODD;
        opt.c_iflag |=INPCK;
        break;
    default:
        qDebug("校验设置失败");
        return (3);
    }
    switch(stopbits){
    case 1:
        opt.c_cflag &= ~CSTOPB;
        break;
    case 2:
        opt.c_cflag |=CSTOPB;
        break;
    default:
        qDebug("停止位设置失败\n");
        return (4);
    }
    tcflush(fd,TCIOFLUSH);

    ret=tcsetattr(fd,TCSANOW,&opt);
    if(ret!=0){
        qDebug("串口设置失败");
        return (5);
    }

    return (0);
}




void Serial::on_pushButton_1_clicked()
{
    QString tim=ui->lineEdit->text();
    tim+="1";
    ui->lineEdit->setText(tim);
}

void Serial::on_pushButton_2_clicked()
{
    QString tim=ui->lineEdit->text();
    tim+="2";
    ui->lineEdit->setText(tim);
}

void Serial::on_pushButton_3_clicked()
{
    QString tim=ui->lineEdit->text();
    tim+="3";
    ui->lineEdit->setText(tim);
}

void Serial::on_pushButton_4_clicked()
{
    QString tim=ui->lineEdit->text();
    tim+="4";
    ui->lineEdit->setText(tim);
}

void Serial::on_pushButton_5_clicked()
{
    QString tim=ui->lineEdit->text();
    tim+="5";
    ui->lineEdit->setText(tim);
}

void Serial::on_pushButton_6_clicked()
{
    QString tim=ui->lineEdit->text();
    tim+="6";
    ui->lineEdit->setText(tim);
}

void Serial::on_pushButton_7_clicked()
{
    QString tim=ui->lineEdit->text();
    tim+="7";
    ui->lineEdit->setText(tim);
}

void Serial::on_pushButton_8_clicked()
{
    QString tim=ui->lineEdit->text();
    tim+="8";
    ui->lineEdit->setText(tim);
}

void Serial::on_pushButton_9_clicked()
{
    QString tim=ui->lineEdit->text();
    tim+="9";
    ui->lineEdit->setText(tim);
}

void Serial::on_pushButton_0_clicked()
{
    QString tim=ui->lineEdit->text();
    tim+="0";
    ui->lineEdit->setText(tim);
}

void Serial::on_pushButton_clr_clicked()
{
    ui->lineEdit->clear();
}

void Serial::on_pushButton_del_clicked()
{
    QString tim=ui->lineEdit->text();
    tim.chop(1);
    ui->lineEdit->setText(tim);

}
