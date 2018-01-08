//#include "serials.h"
#include "GPIO.h"
#include <QApplication>
#include "startui.h"
int fd;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Serials w;
    StartUi w;
    w.showFullScreen();
    w.show();

    return a.exec();
}
