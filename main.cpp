#include "mainwidget.h"
#include "status.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //在程序启动前的函数，进行一些全局数据的计算
    BeforeAppRun();
    //创建状态对象
    Status status;
    return app.exec();
}
