#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QDebug>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->loadCaptions("C:\\Users\\huqin\\Desktop\\trans\\cz");
    this->currentIndex = 0;
    this->logList = new StringList();
    this->ui->logListView->setModel(this->logList);
    //this->ui->chineseTextListView->setModel(this->chineseTextList);
    //this->ui->englishTextListView->setModel(this->englishTextList);
    //this->log("Test");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadCaptions(QString location) {
    this->englishTexts = this->readFile(location + ".en");
    this->chineseTexts = this->readFile(location + ".zh");
    /*
    QStringListModel* englishModel = new QStringListModel(this->englishTexts);

    this->ui->englishTextListView->setModel(englishModel);
    this->ui->englishTextListView->setCurrentIndex(englishModel->index(1,0));
    this->ui->englishTextListView->setWordWrap(true);

    QStringListModel* chineseModel = new QStringListModel(this->chineseTexts);
    this->ui->chineseTextListView->setModel(chineseModel);
    */
    this->ui->translationWidget->setColumnCount(2);
    this->ui->translationWidget->setColumnWidth(0, this->ui->translationWidget->width() * 0.45);
    this->ui->translationWidget->setColumnWidth(1, this->ui->translationWidget->width() * 0.45);
    this->ui->translationWidget->setWordWrap(true);
    for(int i=0;i<this->englishTexts.length();i++){
        this->ui->translationWidget->insertRow(i);
        this->ui->translationWidget->setItem(i, 0, new QTableWidgetItem(this->englishTexts[i]));
    }
    for(int i=0;i<this->chineseTexts.length();i++){
        if(this->ui->translationWidget->rowCount() < i) {
            this->ui->translationWidget->insertRow(i);
        }
        this->ui->translationWidget->setItem(i, 1, new QTableWidgetItem(this->chineseTexts[i]));
    }
    this->ui->translationWidget->resizeRowsToContents();

}

void MainWindow::update() {
    this->ui->translationWidget->selectRow(this->currentIndex);
    this->ui->translationWidget->scrollToItem(this->ui->translationWidget->itemAt(this->currentIndex, 0));
}

void MainWindow::log(QString text) {
    this->logList->append(QString("[") + QTime::currentTime().toString("hh:mm:ss") + QString("] ") + text);
}

QList<QString> MainWindow::readFile(QString filename)
{
    //this->log(QString("Reading file: ") + filename);
    QList<QString> results;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        while (!in.atEnd()) {
            QString line = in.readLine();
            results.append(line);
        }
    }
    //this->log(QString(results.count() + 1) + " line(s) read.");
    file.close();
    return results;
}

void MainWindow::on_nextButton_clicked()
{
    this->currentIndex++;
    this->update();
}
