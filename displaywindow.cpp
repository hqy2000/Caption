#include "displaywindow.h"
#include "ui_displaywindow.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>

DisplayWindow::DisplayWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DisplayWindow)
{
    ui->setupUi(this);
    this->backgroundImageUrl = nullptr;

}

DisplayWindow::~DisplayWindow()
{
    delete ui;
}

void DisplayWindow::changeText(QString text)
{
    this->ui->label->setText(text);
}

void DisplayWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    if(this->backgroundImageUrl != nullptr) {
        QPixmap bkgnd(this->backgroundImageUrl);
        bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
        QPalette palette;
        palette.setBrush(QPalette::Background, bkgnd);
        this->setPalette(palette);
    }
}

void DisplayWindow::changeFontSize(int size)
{
    if (size > 0) {
        auto font = this->ui->label->font();
        font.setPointSize(size);
        this->ui->label->setFont(font);
    }
}

void DisplayWindow::changeBackgroundImage(QString backgroundImageUrl){
    if (backgroundImageUrl != "") {
        this->backgroundImageUrl = backgroundImageUrl;
        QPixmap bkgnd(this->backgroundImageUrl);
        bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
        QPalette palette;
        palette.setBrush(QPalette::Background, bkgnd);
        this->setPalette(palette);
    }
}
