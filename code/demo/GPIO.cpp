#include "GPIO.h"
#include "ui_GPIO.h"
#include <QIcon>
GPIO::GPIO(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GPIO)
{
    ui->setupUi(this);
    Init_combobox_boardType();
   // pthread_mutex_init(&lock,NULL);
    setWindowIcon(QIcon(":/new/prefix1/icon.png"));
   // ui->textEdit->setText("按键选择NULL表示让led自行闪烁，选择具体按键表示通过按键控制led灯亮灭");
}


void GPIO::Dir_out(){
    DIR *dir;
    struct dirent dirinfo;
    struct dirent *tempdir;
    int ret;


    dir = opendir(DIR_PATH);
    if(dir == NULL)
        return ;

    printf("dir_out\n");
    QString str;
    while(((ret = readdir_r(dir,&dirinfo,&tempdir))==0)&&tempdir){
        str.append(dirinfo.d_name);
        str.append(" ");

    }
    ui->textEdit->setText(str);
    closedir(dir);
}

GPIO::~GPIO()
{
    delete ui;
}


void GPIO::Init_combobox_boardType(){
    QFile file(DATA_PATH);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug()<<"Can't open the file!"<<endl;

    }
    else{
        qDebug()<<"already open the file"<<endl;
    }
    QStringList test;
    QString tmp;
    QTextStream in(&file);
    QString line = in.readLine();
     while (!line.isNull()) {
         if(QString::compare(line,"TYPE")==0){
           //   qDebug()<<QString::compare(line,"TYPE")<<endl;
              line=in.readLine();
              test = QStringList()<<line;
              ui->boardType->addItems(test);
          }
          line=in.readLine();
     }
     file.close();
}

void GPIO::on_run_clicked()
{
    int lednum,keynum;
    QFile file(DATA_PATH);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug()<<"Can't open the file in run!"<<endl;
    }

    QString str;
    str = ui->KEY_NUM->currentText();

    QStringList test;
    QString tmp,text;

    text = ui->boardType->currentText();
    QTextStream in(&file);
    int led_i = ui->LED_NUM->currentIndex()+1;
    int key_i = ui->KEY_NUM->currentIndex()+1;
    QString line = in.readLine();
     while (!line.isNull()) {
         if(QString::compare(line,text)==0){
             line = in.readLine();
          if(QString::compare(line,"LED")==0){
              while(led_i>0){
                  line = in.readLine();
                  led_i--;
              }
              lednum=line.toInt();

              line = in.readLine();

         }
          while(QString::compare(line,"KEY")!=0){
              line = in.readLine();
          }
          if(QString::compare(line,"KEY")==0){
              //  qDebug()<<"line:"<<line<<endl;
              while(key_i>0){
                  line=in.readLine();
                  key_i--;
              }
              keynum=line.toInt();
              line = in.readLine();
          }
          break;
         }
         line=in.readLine();

     }
 /*    if(QString::compare(ui->boardType->currentText(),"Others")==0){
         lednum = ui->LED_NUM->currentText().toInt();
         keynum = ui->KEY_NUM->currentText().toInt();
     }*/
     cout<<"led:"<<lednum<<endl;
     cout<<"key:"<<keynum<<endl;
     int ret;
     pthread_t tid;
     board_num_in *temp = new board_num_in;
     if(!temp){
         cout<<"new failed\n"<<endl;
         return ;
     }
    if(QString::compare(str,"NULL")==0){
        keynum = -1;
        ret = pthread_create(&tid,NULL,gpio_out,(void *)lednum);
        if(ret != 0){
            cout<<"create thread failed"<<endl;
            exit(-1);
        }
        ui->textEdit->clear();
        ui->textEdit->append("done");
      }
    else{

        temp->key = keynum;
        temp->led = lednum;

        ret = pthread_create(&tid,NULL,gpio_in,(void*)temp);
        if(ret != 0){
            cout<<"create thread failed"<<endl;
            exit(-1);
        }
       // ui->textEdit->append("key done\n");
    }
    printf("%d %d\n",lednum,keynum);
    //pthread_join(tid,NULL);
    //delete temp;
    file.close();
}
/*
void GPIO::on_pushButton_16_clicked()
{
     Dir_out();
}*/


/*
void GPIO::on_OK_clicked()
{
    QString tmp;
     QStringList test;
     if(ui->boardType->currentText()=="Others"){
         if(ui->LEDButton->isChecked()){
             ui->textEdit->setText("led button\n");
             int num = ui->number_edit->toPlainText().toInt();
             if(num>255){
                 ui->textEdit->setText("the range is 0 to 255");
                 ui->number_edit->clear();
             }
             else{
                 tmp = QString::number(num);
                 test = QStringList()<<tmp;
                 ui->LED_NUM->addItems(test);

             }
         }
         else if(ui->KEYButton->isChecked()){
             ui->textEdit->setText("key button\n");
             int num = ui->number_edit->toPlainText().toInt();
             if(num>255){
                 ui->textEdit->setText("the range is 0 to 255");
                 ui->number_edit->clear();
             }else{
                 tmp = QString::number(num);
                 test = QStringList()<<tmp;
                 ui->KEY_NUM->addItems(test);
             }
         }
     }
     else {
         ui->textEdit->setText("请选择LED或者KEY按钮，本项只允许开发板类型为Others时使用。\n");
     }
}



void GPIO::on_pushButton_3_clicked()
{//键盘1
    QString str = ui->number_edit->toPlainText();
    str += "1";
    ui->number_edit->setText(str);
}

void GPIO::on_pushButton_4_clicked()
{//键盘2
    QString str = ui->number_edit->toPlainText();
    str += "2";
    ui->number_edit->setText(str);

}

void GPIO::on_pushButton_5_clicked()
{//键盘3
    QString str = ui->number_edit->toPlainText();
    str += "3";
    ui->number_edit->setText(str);
}

void GPIO::on_pushButton_6_clicked()
{//键盘4
    QString str = ui->number_edit->toPlainText();
    str += "4";
    ui->number_edit->setText(str);

}

void GPIO::on_pushButton_7_clicked()
{//键盘5
    QString str = ui->number_edit->toPlainText();
    str += "5";
    ui->number_edit->setText(str);
}

void GPIO::on_pushButton_8_clicked()
{//键盘6
    QString str = ui->number_edit->toPlainText();
    str += "6";
    ui->number_edit->setText(str);


}

void GPIO::on_pushButton_9_clicked()
{//键盘7
    QString str = ui->number_edit->toPlainText();
    str += "7";
    ui->number_edit->setText(str);
}

void GPIO::on_pushButton_10_clicked()
{//键盘8
    QString str = ui->number_edit->toPlainText();
    str += "8";
    ui->number_edit->setText(str);


}

void GPIO::on_pushButton_11_clicked()
{//键盘9
    QString str = ui->number_edit->toPlainText();
    str += "9";
    ui->number_edit->setText(str);

}

void GPIO::on_pushButton_13_clicked()
{//键盘0
    QString str = ui->number_edit->toPlainText();
    str += "0";
    ui->number_edit->setText(str);

}

void GPIO::on_pushButton_12_clicked()
{//清除按键
    ui->number_edit->clear();

}

void GPIO::on_pushButton_14_clicked()
{//回退按键
     QString str = ui->number_edit->toPlainText();
     str.chop(1);
     ui->number_edit->setText(str);
}

void GPIO::on_LEDButton_clicked()
{
     ui->number_edit->clear();
}

void GPIO::on_KEYButton_clicked()
{
     ui->number_edit->clear();
}
*/
void GPIO::on_boardType_activated(const QString &arg1)
{
    ui->LED_NUM->clear();
    ui->KEY_NUM->clear();
    QStringList test;
    QString tmp;
    tmp = "NULL";
    test=QStringList()<<tmp;
    ui->KEY_NUM->addItems(test);
    QString text = arg1;

    QFile file(DATA_PATH);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug()<<"Can't open the file!"<<endl;
    }

    int i=0,j=1;
    QTextStream in(&file);
    QString line = in.readLine();
     while (!line.isNull()) {
         if(QString::compare(line,text)==0){
             line = in.readLine();
          if(QString::compare(line,"LED")==0){
              line=in.readLine();
              while(QString::compare(line,"KEY")!=0){
            //  qDebug()<<QString::compare(line,"TYPE")<<endl;
              line=in.readLine();
              tmp = QString::number(i,10);
              test = QStringList()<<tmp;
              ui->LED_NUM->addItems(test);
              i++;
              }
              line = in.readLine();
              line = in.readLine();
              while(QString::compare(line,"#")!=0){
                  line = in.readLine();
                  tmp = QString::number(j,10);
                  test = QStringList()<<tmp;
                  ui->KEY_NUM->addItems(test);
                  j++;
          }
         }
         }
         line=in.readLine();

     }
     file.close();

}

void GPIO::on_cancel_clicked()
{
    cancel_flag++;
    ui->textEdit->setText("取消\n");
}


void GPIO::on_pushButton_clicked()
{
    int lednum,keynum;
    QFile file(DATA_PATH);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug()<<"Can't open the file in run!"<<endl;
    }

    QStringList test;
    QString tmp,text;

    text = ui->boardType->currentText();
    QTextStream in(&file);
    int led_i = ui->LED_NUM->currentIndex()+1;
    QString line = in.readLine();
     while (!line.isNull()) {
         if(QString::compare(line,text)==0){
             line = in.readLine();
          if(QString::compare(line,"LED")==0){
              while(led_i>0){
                  line = in.readLine();
                  led_i--;
              }
              lednum=line.toInt();

              line = in.readLine();

         }


          line = in.readLine();

          break;
         }
         line=in.readLine();
     }
     cout<<"led:"<<lednum<<endl;

     int ret;
     pthread_t tid;
     board_num_in *temp = new board_num_in;
     if(!temp){
         cout<<"new failed\n"<<endl;
         return ;
     }

        keynum = -1;
        ret = pthread_create(&tid,NULL,gpio_out,(void *)lednum);
        if(ret != 0){
            cout<<"create thread failed"<<endl;
            exit(-1);
        }
        ui->textEdit->clear();
        ui->textEdit->append("done");
        file.close();
}
