#include "Sonion.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Sonion w;
    w.show();
    return a.exec();
}
