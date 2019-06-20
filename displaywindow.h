#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QMainWindow>

namespace Ui {
class DisplayWindow;
}

class DisplayWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DisplayWindow(QWidget *parent = nullptr);
    ~DisplayWindow();
    void changeText(QString text);
    void resizeEvent(QResizeEvent* event) override;
    void changeBackgroundImage(QString backgroundImageUrl);
    void changeFontSize(int size);

private:
    Ui::DisplayWindow *ui;
    QString backgroundImageUrl;
};

#endif // DISPLAYWINDOW_H
