#include "filemanager.h"
#include<QTextCodec>
#include<QDateTime>
#include<QMessageBox>
FileViewWidget::FileViewWidget(QWidget *parent/*=0*/) :QDialog(parent)
{

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);

    //-----实例化对象
    fileLineEdit = new QLineEdit("/", this);
    fileListWidget = new QListWidget(this);
    buttonBack = new QPushButton(QObject::tr("返回"));
    buttonNewFloder = new QPushButton(QObject::tr("新建文件夹"));
    buttonNewFile = new QPushButton(QObject::tr("新建文件"));
    buttonDelFloder = new QPushButton(QObject::tr("删除文件夹"));
    buttonDelFile = new QPushButton(QObject::tr("删除文件"));
    buttonEnter = new QPushButton(QObject::tr("进入"));
    buttonIndex = new QPushButton(QObject::tr(""));


    //---布局
    vLayout = new QVBoxLayout(this);
    vHlayout = new QHBoxLayout(this);
    vLayout->addWidget(fileLineEdit);
    vLayout->addLayout(vHlayout);
    vHlayout->addWidget(buttonIndex);
    vLayout->addWidget(fileListWidget);
    vHlayout->addWidget(buttonBack);
    vHlayout->addWidget(buttonNewFloder);
    vHlayout->addWidget(buttonNewFile);
    vHlayout->addWidget(buttonDelFloder);
    vHlayout->addWidget(buttonDelFile);
    vHlayout->addWidget(buttonEnter);

    //--设置对应信号与槽
    connect(fileLineEdit, SIGNAL(returnPressed()),
                this, SLOT(slotDirShow(QDir)));
    connect(fileListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
                this, SLOT(slotDirShow(QListWidgetItem*)));
    connect(fileListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
                this, SLOT(slotDirName(QListWidgetItem*)));
    connect(buttonBack, SIGNAL(clicked()),
                this, SLOT(slotDirBack()));
    connect(buttonNewFloder, SIGNAL(clicked()),
                this, SLOT(newDFile()));
    connect(buttonNewFile, SIGNAL(clicked()),
                this, SLOT(newTxtFile ()));
    connect(buttonDelFloder, SIGNAL(clicked()),
                this, SLOT(removeFolderContent()));
    connect(buttonDelFile, SIGNAL(clicked()),
                this, SLOT(deleteFile()));
    connect(buttonIndex, SIGNAL(clicked()),
                this, SLOT(returnIndex()));
    connect(buttonEnter, SIGNAL(clicked()),
                this, SLOT(slotEnter()));

    QString rootStr = "/";
    QDir rootDir(rootStr);
    QStringList stringlist;
    stringlist << "*";
    list = rootDir.entryInfoList(stringlist);
    showFileInfoList(list);
    buttonIndex->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/cottage.png);}");
    buttonIndex->setFixedSize(30,30);

    //---显示布局
    fileListWidget->setStyleSheet("QListWidget{border:1px solid gray; color:#6E6E6E; }"
                               "QListWidget::Item{padding-top:10px; padding-bottom:10px; }"
                               "QListWidget::Item:hover{background:skyblue; }"
                               "QListWidget::item:selected{background:lightgray; color:red;border:none }"
                               "QListWidget::item:selected:!active{border-width:0px; background:lightgreen; }"
                               );
    setLayout(vLayout);

    //----设置窗口属性
    setWindowTitle("文件管理功能展示");
    setWindowIcon(QIcon(":/new/prefix1/icon.png"));
    setBackgroundRole(QPalette::Dark);
}

//--显示当前目录下的所有文件
void FileViewWidget::slotShow(QDir dir)
{
     QStringList stringList;
     stringList << "*";
     QFileInfoList InfoList = dir.entryInfoList(stringList, QDir :: AllEntries, QDir :: DirsFirst);
     showFileInfoList(InfoList);
}

//---用双击浏览器中显示的目录进入下一级。
void FileViewWidget::showFileInfoList(QFileInfoList list)
{
    //--清空列表控件
    fileListWidget->clear();

    //----取出所有项，按照目录，文件方式添加到控件内
    for (unsigned int i = 0; i < list.count(); i++)
    {
        QFileInfo tmpFileInfo = list.at(i);
        if (tmpFileInfo.isDir())
        {
            QIcon icon(":/new/prefix1/dir.png");
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem*tmpListWidgetItem = new QListWidgetItem(icon, fileName);
            fileListWidget->addItem(tmpListWidgetItem);
        }
        else
        {
            QIcon icon(":/new/prefix1/file.png");
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem*tmpListWidgetItem = new QListWidgetItem(icon, fileName);
            fileListWidget->addItem(tmpListWidgetItem);
        }
    }
}

//----根据用户的选择显示下一级目录下的文件，
void FileViewWidget::slotDirShow(QListWidgetItem *Item)
{
    //----保存下一级目录名
    string = Item->text();
    //string2 = fileLineEdit->text()+string;
    //----设置路径为当前目录路径
    dir.setPath(fileLineEdit->text());
    //-----重新设置路径
    dir.cd(string);
    //----更新当前显示路径
    fileLineEdit->setText(dir.absolutePath()+"/");
    //---显示当前文件目录下的所有文件
    slotShow(dir);
}

void FileViewWidget::slotDirName(QListWidgetItem *Item)
{
    //----保存点击目录名
    string = Item->text();
    string2 = fileLineEdit->text()+Item->text();
}

void FileViewWidget::slotEnter()
{
    //----设置路径为当前目录路径
    dir.setPath(fileLineEdit->text());
    //-----重新设置路径
    dir.cd(string);
    //----更新当前显示路径
    fileLineEdit->setText(dir.absolutePath()+"/");
    QDir dir1(string2);
    slotShow(dir1);
}
//----根据用户的选择返回上一级目录下的文件，
void FileViewWidget::slotDirBack()
{
    //----设置路径为当前目录路径
    dir.setPath(fileLineEdit->text());
    //-----重新设置路径
    dir.cd("..");
    //----更新当前显示路径， 这里获取的是绝对路径
    fileLineEdit->setText(dir.absolutePath()+"/");
    //---显示当前文件目录下的所有文件
    slotShow(dir);
}

//新建文件夹
void FileViewWidget::newDFile()
{
    QDateTime datetime;
    QString fullPath = fileLineEdit->text() + "文件夹" + datetime.currentDateTime().toString("yyyyMMddHHmmss");

    QDir dir(fullPath);
       if(dir.exists())
       {
            dir.setPath(fullPath);
            slotShow(dir);
       }
       else
       {
          dir.mkdir(fullPath);//只创建一级子目录，即必须保证上级目录存在
          dir.setPath(fileLineEdit->text());
          slotShow(dir);
       }

}

//新建文件
void FileViewWidget::newTxtFile ()
{
    QDateTime datetime;
    dir.setPath(fileLineEdit->text());
    QString timestr=datetime.currentDateTime().toString("yyyyMMddHHmmss");
    fileName =fileLineEdit->text()+ "文件" + timestr + ".txt";
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text );
    file.flush();
    file.close();
    //buttonBack->setText(fileLineEdit->text());
    slotShow(dir);
}


//删除文件夹
void FileViewWidget::removeFolderContent()
{
    QString path = string2;
    QDateTime datetime;
    dir.setPath(fileLineEdit->text());

    QString timestr=datetime.currentDateTime().toString("yyyyMMddHHmmss");
    fileName =fileLineEdit->text()+ "文件" + timestr + ".txt";
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text );
    file.flush();
    file.close();

    DeleteDirectory(path);

    QFile::remove(fileName );
    slotShow(dir);
}

//删除文件
void FileViewWidget::deleteFile()
{
     QFile::remove(fileLineEdit->text()+fileListWidget->currentItem()->text() );
     QMessageBox::information(this, "click", fileListWidget->currentItem()->text());
     dir.setPath(fileLineEdit->text());
     slotShow(dir);
}

//返回根目录
void FileViewWidget::returnIndex()
{
    fileLineEdit->setText("/");
    dir.setPath(fileLineEdit->text());
    slotShow(dir);
    //this->close();
}

//删除指定文件夹
bool FileViewWidget::DeleteDirectory(const QString &path)
{
    if (path.isEmpty())
        return false;

    QDir dir(path);
    if(!dir.exists())
        return true;
    QMessageBox::information(this, "删除", path);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo fi, fileList)
    {
        if (fi.isFile())
            fi.dir().remove(fi.fileName());
        else
           DeleteDirectory(fi.absoluteFilePath());
    }
    return dir.rmpath(dir.absolutePath());
}


