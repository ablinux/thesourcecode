#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <stdint.h>
#include <QMainWindow>
#include <windows.h>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString FileName;
    uint32_t FileSize;
    HANDLE hCom;

private slots:
    void on_searchFile_triggered(QAction *arg1);

    void on_searchFile_clicked();

    void on_selecFile_clicked();

    void on_downloadProgressBar_valueChanged(int value);

    void on_pushButton_clicked();

    void on_SelectComPort_clicked();

    void get_ComPorts();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
