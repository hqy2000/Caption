#include "displaywindow.h"
#include "ui_displaywindow.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>

DisplayWindow::DisplayWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DisplayWindow)
{
    ui->setupUi(this);

    QPixmap pix;
   if(pix.load("C:/Users/huqin/Desktop/2019-06-19_22.11.04.png")){
       /** scale pixmap to fit in label'size and keep ratio of pixmap */
       pix = pix.scaled(this->ui->label->size(), Qt::KeepAspectRatio);
       //this->ui->label->setPixmap(pix);
       //->ui->label->setStyleSheet("background-image: url(C:/Users/huqin/Desktop/2019-06-19_22.11.04.png);");
   }
}

DisplayWindow::~DisplayWindow()
{
    delete ui;
}

void DisplayWindow::changeText(QString text)
{
    this->ui->label->setText(text);
}
