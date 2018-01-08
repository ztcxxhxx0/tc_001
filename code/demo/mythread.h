#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <unistd.h>
extern int fd;


class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QThread *parent = 0);
    void ok();
    void shutdown();
    QString buff_out;
protected:

signals:
    void stringChanged(const QString &);//const QString &);
private:
    void run();
    volatile bool waitit;
    volatile bool stopit;

public slots:
};

#endif // MYTHREAD_H
