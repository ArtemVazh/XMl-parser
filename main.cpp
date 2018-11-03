#include "xmlparser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XmlParser w;
    w.show();

    return a.exec();
}
