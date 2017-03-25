#include "sun.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Sun w;
    w.show();

    return a.exec();
}
