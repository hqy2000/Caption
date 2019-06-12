#include "displaywindow.h"
#include "ui_displaywindow.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>

DisplayWindow::DisplayWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DisplayWindow)
{
    ui->setupUi(this);


    QMediaPlayer* player = new QMediaPlayer;

    QMediaPlaylist* playlist = new QMediaPlaylist(player);
    playlist->addMedia(QUrl::fromLocalFile("C:/Users/huqin/Desktop/videoplayback (1).mp4"));
    player->setVideoOutput(this->ui->videoWidget);
    this->ui->videoWidget->show();
    playlist->setCurrentIndex(0);
    player->play();

}

DisplayWindow::~DisplayWindow()
{
    delete ui;
}

void DisplayWindow::changeText(QString text)
{
    this->ui->label->setText(text);
}
