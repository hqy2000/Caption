#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stringlist.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_nextButton_clicked();

private:
    Ui::MainWindow *ui;
    QList<QString> readFile(QString filename);
    void loadCaptions(QString location);
    void update();
    void log(QString text);
    int currentIndex;
    StringList* logList;
    QList<QString> englishTexts;
    StringList* englishTextList;
    QList<QString> chineseTexts;
    StringList* chineseTextList;
};

#endif // MAINWINDOW_H
