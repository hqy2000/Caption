#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "displaywindow.h"
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include <QSettings>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->writeSampleConfigFile();
    this->readConfigFile("config_sample.ini");
    this->currentIndex = 0;
    this->logList = new StringList();
    this->ui->logListView->setModel(this->logList);
    this->isDisplayingEnglish = false;
    this->update(true);

    this->displayWindow.show();
    //this->log("Test");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadCaptions(QString location) {
    this->englishTexts = this->readFile(location + ".en");
    this->chineseTexts = this->readFile(location + ".zh");
    this->ui->translationWidget->setColumnCount(2);
    this->ui->translationWidget->setColumnWidth(0, this->ui->translationWidget->width() * 0.45);
    this->ui->translationWidget->setColumnWidth(1, this->ui->translationWidget->width() * 0.45);
    this->ui->translationWidget->setWordWrap(true);
    if(this->englishTexts.count() == this->chineseTexts.count()) {
        for(int i=0;i<this->englishTexts.length();i++){
            this->ui->translationWidget->insertRow(i);
            this->ui->translationWidget->setItem(i, 0, new QTableWidgetItem(this->englishTexts[i]));
            this->ui->translationWidget->setItem(i, 1, new QTableWidgetItem(this->chineseTexts[i]));
        }
        this->ui->translationWidget->resizeRowsToContents();
    }
    this->ui->progressBar->setRange(0, this->englishTexts.count() - 1);

}

void MainWindow::update(bool isForward = true) {
    this->ui->translationWidget->selectRow(this->currentIndex);
    if (isForward)
        if(this->currentIndex >= this->ui->translationWidget->rowCount() - 2)
            this->ui->translationWidget->scrollToBottom();
        else
            this->ui->translationWidget->scrollToItem(this->ui->translationWidget->itemAt(this->currentIndex + 3, 0));
    else
        if(this->currentIndex <= 2)
            this->ui->translationWidget->scrollToTop();
        else
            this->ui->translationWidget->scrollToItem(this->ui->translationWidget->itemAt(this->currentIndex - 3, 0));

    if(this->isDisplayingEnglish) {
        this->ui->currentTextBrowser->setText(this->englishTexts[currentIndex]);
        this->displayWindow.changeText(this->englishTexts[currentIndex]);
    } else {
        this->ui->currentTextBrowser->setText(this->chineseTexts[currentIndex]);
        this->displayWindow.changeText(this->chineseTexts[currentIndex]);
    }

    this->ui->progressBar->setValue(this->currentIndex);
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
    if (results.count() == 0) {
        QMessageBox(QMessageBox::Warning, "Warning", "Read file error!").exec();
    }
    //this->log(QString(results.count() + 1) + " line(s) read.");
    file.close();
    return results;
}

void MainWindow::readConfigFile(QString configFile)
{
    QSettings* config = new QSettings(configFile, QSettings::IniFormat);
    config->setIniCodec("UTF-8");
    this->ui->chineseNameLabel->setText(config->value("ChineseName").toString());
    this->ui->englishNameLabel->setText(config->value("EnglishName").toString());
    this->ui->chineseClassLabel->setText(config->value("ChineseClass").toString());
    this->ui->englishClassLabel->setText(config->value("EnglishClass").toString());
    if(config->value("Display").toString() == "Chinese") {
        this->isDisplayingEnglish = false;
    } else {
        this->isDisplayingEnglish = true;
    }
    this->loadCaptions(config->value("File").toString());
}

void MainWindow::writeSampleConfigFile()
{
    QSettings* config = new QSettings(QDir::currentPath() + "/config_sample.ini", QSettings::IniFormat);
    config->setIniCodec("UTF-8");
    config->setValue("ChineseName", "雷杆子");
    config->setValue("EnglishName", "Leigh Smith");
    config->setValue("ChineseClass", "外方校长");
    config->setValue("EnglishClass", "Center Principal");
    config->setValue("Display", "Chinese");
    config->setValue("File", "C:/Users/huqin/Desktop/trans/cz");
    config->sync();
}
void MainWindow::on_nextButton_clicked()
{
    this->currentIndex++;
    if(this->currentIndex >= this->englishTexts.count()) {
        QMessageBox(QMessageBox::Warning, "Warning", "Already at last.", QMessageBox::Ok).exec();
        this->currentIndex = this->englishTexts.count() - 1;
    }
    this->update(true);
}

void MainWindow::on_previousButton_clicked()
{
    this->currentIndex--;
    if(this->currentIndex < 0) {
        QMessageBox(QMessageBox::Warning, "Warning", "Already at beginning.", QMessageBox::Ok).exec();
        this->currentIndex = 0;
    }
    this->update(false);
}
