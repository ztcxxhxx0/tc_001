#ifndef FILEMANAGER
#define FILEMANAGER
#include <QWidget>
#include <QDir>
#include <QListWidgetItem>
#include <QFileInfoList>
#include <QListWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QIcon>
#include <QStringList>
#include <QPushButton>
#include <QTextCodec>
#include <QDialog>

class FileViewWidget : public QDialog
{
    Q_OBJECT
public:
    FileViewWidget(QWidget *parent=0);

    //---用双击浏览器中显示的目录进入下一级，或者返回上一级目录。
    void showFileInfoList(QFileInfoList list);
    bool DeleteDirectory(const QString &path);


public slots:
        //--显示当前目录下的所有文件
        void slotShow(QDir dir);

        //----根据选择显示下一级目录下的文件
        void slotDirShow(QListWidgetItem *Item);
        void slotEnter();
        //----根据选择显示当前文件夹名称
        void slotDirName(QListWidgetItem *Item);

        //----返回上一级目录
        void slotDirBack();

        //----新建文件夹
        void newDFile();

        //----新建文件
        void newTxtFile ();

        //----删除文件夹
        void removeFolderContent();

        //----删除文件
        void deleteFile();

        //----返回首页
        void returnIndex();

private:
    QLineEdit *fileLineEdit;
    QListWidget *fileListWidget;
    QVBoxLayout *vLayout;
    QHBoxLayout *vHlayout;
    QFileInfoList list;
    QDir dir;
    QString string;
    QString string2;
    QPushButton *buttonBack;
    QPushButton *buttonNewFloder;
    QPushButton *buttonNewFile;
    QPushButton *buttonDelFloder;
    QPushButton *buttonDelFile;
    QPushButton *buttonIndex;
    QPushButton *buttonEnter;
    QString fileName;
};
#endif // FILEMANAGER

