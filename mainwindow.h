#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <Windows.h>
#include <Windowsx.h>
#include <map>
#include <barchart.h>
#include <heatmap.h>
#include <QImage>
#include <QScreen>
#include<QFuture>
#include <QtConcurrent/QtConcurrent>
#pragma comment(lib, "user32.lib")
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void updateClicks(DWORD xPos, DWORD yPos,WPARAM wParam);
    void updatePresses(WPARAM wParam,LPARAM lParam);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    int totalElaspedSeconds = 0;
    int totalPresses = 0;
    int totalClicks = 0;
    int totalMouseMoves = 0;
    std::map <DWORD,int> alphabetMap;
    std::map <std::pair<DWORD,DWORD>,int> mousePosMap;
    BarChart *alphabetChart;
    bool keyDown = true;
    bool sessionIsRunning = false;
    DWORD lastVkCode = 0;
    void runConcurrentHeatmap();

private slots:
    void updateTimer();
    void on_startSessionButton_clicked();
    void on_createHeatMapButton_clicked();
    void on_newSessionButton_clicked();
};

#endif // MAINWINDOW_H
