#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stringlist.h>
#include <displaywindow.h>

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

    void on_previousButton_clicked();

private:
    Ui::MainWindow *ui;
    QList<QString> readFile(QString filename);
    void loadCaptions(QString location);
    void update(bool isForward);
    void log(QString text);
    void readConfigFile(QString configFile);
    void writeSampleConfigFile();
    int currentIndex;
    bool isDisplayingEnglish;
    StringList* logList;
    QList<QString> englishTexts;
    StringList* englishTextList;
    QList<QString> chineseTexts;
    StringList* chineseTextList;
    DisplayWindow displayWindow;
};

#endif // MAINWINDOW_H
