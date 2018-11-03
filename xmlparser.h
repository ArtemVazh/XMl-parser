#include <QMainWindow>
#include <QtWidgets>
#include <QtXml>
#include <QtXmlPatterns>
#include <fstream>
#include "xmltreemodel.h"

namespace Ui {
class XmlParser;
}

class XmlParser : public QMainWindow
{
    Q_OBJECT

public:
    explicit XmlParser(QWidget *parent = 0);
    ~XmlParser();
private:
    QList<QString> fileNames;
    Ui::XmlParser *ui;
    void CreateMenu();
    void SetupModel();
private slots:
    void XQueryFromFile();
    void XQuery();
    void Close();
    void OpenFile();
    void closeFile();
};
