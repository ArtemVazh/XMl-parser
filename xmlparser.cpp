#include "xmlparser.h"
#include "ui_xmlparser.h"

XmlParser::XmlParser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::XmlParser)
{
    ui->setupUi(this);
    CreateMenu();
    this->resize(500, 500);
    this->setWindowIcon(QIcon("XML-integration.jpg"));
}

XmlParser::~XmlParser()
{
    delete ui;
}

void XmlParser::CreateMenu(){
    QMenu *file = new QMenu("&Файл");
    file->setStyleSheet(
                "QMenu {"
                "border: 1px solid navy;"
                "font: bold 14px;}");
    ui->menuBar->addMenu(file);
    QAction *Open = new QAction("&Открыть", file);
    connect(Open, SIGNAL(triggered()), this, SLOT(OpenFile()));
    file->addAction(Open);
    QAction *Close = new QAction("&Закрыть все", file);
    connect(Close, SIGNAL(triggered()), this, SLOT(closeFile()));
    file->addAction(Close);
    ui->menuBar->addSeparator();
    QMenu *query = new QMenu("&X-Query");
    ui->menuBar->addMenu(query);
    query->setStyleSheet(
                "QMenu {"
                "border: 1px solid navy;"
                "font: bold 14px;}");
    QAction *input = new QAction("Ввести X-Query запрос");
    connect(input, SIGNAL(triggered()), this, SLOT(XQuery()));
    query->addAction(input);
    QAction *fromfile = new QAction("Выполнить X-Query запрос");
    connect(fromfile, SIGNAL(triggered()), this, SLOT(XQueryFromFile()));
    query->addAction(fromfile);
    QAction *Exit = new QAction("&Выход");
    connect(Exit, SIGNAL(triggered()), this, SLOT(Close()));
    ui->menuBar->addAction(Exit);
    ui->menuBar->setStyleSheet(
                "QMenuBar {"
                "border: 1px solid navy;"
                "font: bold 14px;}");
}

void XmlParser::XQuery(){
    QString strQuery = QInputDialog::getText(this, QString::fromUtf8("Введите XQuery запрос"), QString::fromUtf8("Ваш XQuery запрос:"), QLineEdit::Normal);
    QString xmlfileName;
    if(fileNames.size() != 1)
        xmlfileName = QFileDialog::getOpenFileName( this, tr("Выберите файл xml"), "/Users/artem/Desktop/Ucheba (for Qt)/3 semestr/C++/DZ/XML", tr("Document files (*.xml *.xq);;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog );
    else
        xmlfileName = fileNames[0];
    QFile xmlFile(xmlfileName);
    if (xmlFile.open(QIODevice::ReadOnly)){
        QXmlQuery query;
        query.bindVariable("inputDocument", &xmlFile);
        query.setQuery(strQuery);
        if (!query.isValid()) {
            QMessageBox::critical(this, "Ошибка", "Запрос не верный!", QMessageBox::Ok);
            return;
        }
        QString strOutput;
        if(!query.evaluateTo(&strOutput)){
            QMessageBox::critical(this, "Ошибка", "Can't evaluate the query!", QMessageBox::Ok);
            return;
        }
        xmlFile.close();
        if(strOutput.contains("<")) {
            std::ofstream resultFile("result.xml", std::ios::trunc);
            resultFile << "<?xml version=\"1.0\"?>";
            resultFile << "<Result>";
            resultFile << strOutput.toStdString();
            resultFile << "</Result>";
            resultFile.close();
            QList<QString> names;
            names << "result.xml";
            XmlTreeModel *model = new XmlTreeModel(names);
            QFile file("/result.xml");
            QXmlInputSource inputSource(&file);
            QXmlSimpleReader reader;
            reader.setContentHandler(model);
            reader.setErrorHandler(model);
            reader.parse(inputSource);
            QTreeView *treeView = new QTreeView;
            treeView->setModel(model);
            treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
            treeView->header()->setStretchLastSection(true);
            treeView->header()->hide();
            treeView->setStyleSheet(
                    "QTreeView {"
                    "min-width: 30em;"
                    "font: bold 14px;}");
            treeView->setWindowTitle("Результат X-Query запроса");
            treeView->show();
        }
        else {
            QMessageBox::about(this, "Результат X-Query запроса", strOutput);
        }
    }
}

void XmlParser::XQueryFromFile(){
    QString xqfileName = QFileDialog::getOpenFileName( this, tr("Выберите файл с запросом"), "/Users/artem/Desktop/Ucheba (for Qt)/3 semestr/C++/DZ/XML", tr("Document files (*.xml *.xq);;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog );
    QFile xqFile(xqfileName);
    QString strQuery;
    if (xqFile.open(QIODevice::ReadOnly)){
        strQuery = xqFile.readAll();
        xqFile.close();
    }
    else {
        QMessageBox::critical(this, "Ошибка", "Файл не открыт!", QMessageBox::Ok);
        return;
    }
    QString xmlfileName;
    if(fileNames.size() != 1)
        xmlfileName = QFileDialog::getOpenFileName( this, tr("Выберите файл xml"), "/Users/artem/Desktop/Ucheba (for Qt)/3 semestr/C++/DZ/XML", tr("Document files (*.xml *.xq);;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog );
    else
        xmlfileName = fileNames[0];
    QFile xmlFile(xmlfileName);
    if (xmlFile.open(QIODevice::ReadOnly)){
        QXmlQuery query;
        query.bindVariable("inputDocument", &xmlFile);
        query.setQuery(strQuery);
        if (!query.isValid()) {
            QMessageBox::critical(this, "Ошибка", "Запрос не верный!", QMessageBox::Ok);
            return;
        }
        QString strOutput;
        if(!query.evaluateTo(&strOutput)){
            QMessageBox::critical(this, "Ошибка", "Can't evaluate the query!", QMessageBox::Ok);
            return;
        }
        xmlFile.close();
        if(strOutput.contains("<")) {
            std::ofstream resultFile("result.xml", std::ios::trunc);
            resultFile << "<?xml version=\"1.0\"?>";
            resultFile << "<Result>";
            resultFile << strOutput.toStdString();
            resultFile << "</Result>";
            resultFile.close();
            QList<QString> names;
            names << "result.xml";
            XmlTreeModel *model = new XmlTreeModel(names);
            QFile file("/result.xml");
            QXmlInputSource inputSource(&file);
            QXmlSimpleReader reader;
            reader.setContentHandler(model);
            reader.setErrorHandler(model);
            reader.parse(inputSource);
            QTreeView *treeView = new QTreeView;
            treeView->setModel(model);
            treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
            treeView->header()->setStretchLastSection(true);
            treeView->header()->hide();
            treeView->setStyleSheet(
                    "QTreeView {"
                    "min-width: 30em;"
                    "font: bold 14px;}");
            treeView->setWindowTitle("Результат X-Query запроса");
            treeView->show();
        }
        else {
            QMessageBox::about(this, "Результат X-Query запроса", strOutput);
        }
    }
}

void XmlParser::OpenFile()
{
    QString fileName = QFileDialog::getOpenFileName( this, tr("Открыть файл"), "/Users/artem/Desktop/Ucheba (for Qt)/3 semestr/C++/DZ/XML", tr("Document files (*.xml);;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog );
    if(!fileNames.contains(fileName))
        fileNames << fileName;
    SetupModel();
}

void XmlParser::Close()
{
    this->close();
}

void XmlParser::closeFile()
{
    qDeleteAll(ui->centralWidget->children());
    fileNames.clear();
}

void XmlParser::SetupModel(){
    if(fileNames.size() > 1)
        qDeleteAll(ui->centralWidget->children());
    if(fileNames.empty())
        return;
    else {
        XmlTreeModel *model = new XmlTreeModel(fileNames);
        for (int i = 0; i < fileNames.size(); ++i) {
            QFile file(fileNames[i]);
            QXmlInputSource inputSource(&file);
            QXmlSimpleReader reader;
            reader.setContentHandler(model);
            reader.setErrorHandler(model);
            reader.parse(inputSource);
        }
        QTreeView *treeView = new QTreeView;
        treeView->setModel(model);
        treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        treeView->header()->setStretchLastSection(true);
        treeView->header()->hide();
        treeView->setStyleSheet(
                "QTreeView {"
                "min-width: 30em;"
                "font: bold 14px;}");
        QHBoxLayout *TreeLayout = new QHBoxLayout;
        TreeLayout->addWidget(treeView);
        ui->centralWidget->setLayout(TreeLayout);
    }
}


