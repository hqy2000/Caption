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
#include <QFileDialog>
#include <QCloseEvent>
#include <QWindow>
#include<QTimer>


MainWindow::MainWindow(QWidget *parent, QString configFileUrl) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->writeSampleConfigFile();
    this->load(configFileUrl);
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadCaptions(QString location) {
    this->englishTexts = this->readFile(location + ".en");
    this->chineseTexts = this->readFile(location + ".zh");
    this->ui->translationWidget->setColumnCount(2);
    if(this->isDisplayingEnglish) {
        this->ui->translationWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Display"));
        this->ui->translationWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Reference"));
    } else {
        this->ui->translationWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Refrence"));
        this->ui->translationWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Display"));
    }

    this->ui->translationWidget->setColumnWidth(0, this->ui->translationWidget->width() * 0.46);
    this->ui->translationWidget->setColumnWidth(1, this->ui->translationWidget->width() * 0.46);
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
    this->ui->translationWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::load(QString config)
{
    this->displayWindow.hide();
    this->isDisplayingEnglish = false;
    while(this->englishTexts.count() == 0 || this->chineseTexts.count() == 0) {
        if(config == nullptr) {
            QString fileName = QFileDialog::getOpenFileName(this,
                tr("Open Config"), QDir::currentPath(), tr("Config Files (*.ini *.conf)"));
            if (fileName == "") {
                QTimer::singleShot(0, qApp, &QCoreApplication::quit);
                break;
            }
            this->readConfigFile(fileName);
        } else {
            this->readConfigFile(config);
            config = nullptr;
        }
    }
    this->currentIndex = 0;
    this->startTime = 0;
    this->ui->nextButton->setDisabled(true);
    this->ui->previousButton->setDisabled(true);
    this->update(true);
    this->displayWindow.show();
    if (qApp->screens().count() > 1) {
        this->displayWindow.windowHandle()->setScreen(qApp->screens()[1]);
        this->displayWindow.showFullScreen();
    }

}

void MainWindow::update(bool isForward = true) {
    this->ui->translationWidget->selectRow(this->currentIndex);
    if (isForward)
        if(this->currentIndex >= this->ui->translationWidget->rowCount())
            this->ui->translationWidget->scrollToBottom();
        else
            this->ui->translationWidget->scrollToItem(this->ui->translationWidget->item(this->currentIndex + 1, 0));
    else
        if(this->currentIndex <= 0)
            this->ui->translationWidget->scrollToTop();
        else
            this->ui->translationWidget->scrollToItem(this->ui->translationWidget->item(this->currentIndex - 1, 0));
    if(this->startTime == 0) {
        this->ui->currentTextBrowser->setText(this->ui->chineseNameLabel->text() + "\n" + this->ui->englishNameLabel->text() + "\n" + this->ui->chineseClassLabel->text() + "\n" + this->ui->englishClassLabel->text());
    } else if(this->isDisplayingEnglish) {
        this->ui->currentTextBrowser->setText(this->englishTexts[currentIndex]);
    } else {
        this->ui->currentTextBrowser->setText(this->chineseTexts[currentIndex]);
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
        this->ui->displayLanguageLabel->setText("英文发言/中文字幕");
    } else {
        this->isDisplayingEnglish = true;
        this->ui->displayLanguageLabel->setText("中文发言/英文字幕");
    }
    this->displayWindow.changeFontSize(config->value("FontSize").toInt());
    this->displayWindow.changeBackgroundImage(config->value("Background").toString());
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
    config->setValue("Display", "English");
    config->setValue("File", "D:/毕业典礼/trans/cz");
    config->setValue("Background", "C:/Users/huqin/Documents/ShareX/Screenshots/2019-06/POWERPNT_y09dTL6vPW.jpg");
    config->setValue("FontSize", 96);
    config->sync();
}
void MainWindow::on_nextButton_clicked()
{
    this->writeSubtitle();
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

void MainWindow::on_loadButton_clicked()
{
    this->load();
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Caption",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        this->displayWindow.close();
        event->accept();
    }
}

void MainWindow::writeSubtitle() {
    QFile file(QString::number(this->startTime) + ".srt");
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    QString startTime = QDateTime::fromTime_t((this->lastTime - this->startTime) / 1000).toUTC().toString("hh:mm:ss") + "," + QString::number((this->lastTime - this->startTime) % 1000).rightJustified(3, '0');
    QString stopTime = QDateTime::fromTime_t((currentTime - this->startTime) / 1000).toUTC().toString("hh:mm:ss") + "," + QString::number((currentTime - this->startTime) % 1000).rightJustified(3, '0');
    if(file.open(QFile::Append | QFile::Text)) {
       QTextStream stream(&file);
       stream << this->currentIndex + 1 << "\n";
       stream << startTime << " --> " << stopTime << "\n";
       stream << this->ui->currentTextBrowser->toPlainText() << "\n";
       stream << "\n";
       file.close();
    }
    this->lastTime = currentTime;
}

void MainWindow::on_controlButton_clicked()
{
    if(this->startTime == 0) {
        this->startTime = QDateTime::currentMSecsSinceEpoch();
        this->lastTime = this->startTime;
        this->ui->nextButton->setDisabled(false);
        this->ui->previousButton->setDisabled(false);
        this->ui->controlButton->setText("Stop");
        this->update(false);
    } else {
        this->startTime = 0;
        this->currentIndex = 0;
        this->ui->nextButton->setDisabled(true);
        this->ui->previousButton->setDisabled(true);
        this->ui->controlButton->setText("Start");
        this->update(true);
    }
}

void MainWindow::on_currentTextBrowser_textChanged()
{
    this->displayWindow.changeText(this->ui->currentTextBrowser->toPlainText());
}
