#include "mainwindow.h"
#include "ui_mainwindow.h"

HHOOK mouseHook = NULL;
HHOOK keyboardHook = NULL;
MainWindow *wHook;
KBDLLHOOKSTRUCT kbdStruct;
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    auto xPos = GET_X_LPARAM(GetMessagePos());
    auto yPos = GET_Y_LPARAM(GetMessagePos());
    wHook->updateClicks(xPos,yPos,wParam);
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);

}
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {

    wHook->updatePresses(wParam,lParam);
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    
    wHook = this;
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    ui->setupUi(this);
    alphabetChart = new BarChart(alphabetMap,totalPresses);
    ui->verticalLayout->addWidget(alphabetChart);
    ui->heatmapLabel->setScaledContents(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::lightGray);
    ui->heatmapLabel->setAutoFillBackground(true);
    ui->heatmapLabel->setPalette(pal);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateClicks(DWORD xPos, DWORD yPos,WPARAM wParam)
{
    qDebug() << xPos;
    switch( wParam )
    {

    case WM_LBUTTONDOWN:
          ++totalClicks;
        break;
    case WM_RBUTTONDOWN:

          ++totalClicks;
        break;
    case WM_MBUTTONDOWN:

         ++totalClicks;
        break;
    case WM_MOUSEMOVE:
        ++totalMouseMoves;
        break;
    }
    if (sessionIsRunning) {

        ui->totalClicksText->setText(QString::number(totalClicks));
        ui->totalClicksText->update(); // THIS IS NEEDED OR IT DOESN't CATCH MOUSE WTF
        mousePosMap[std::pair<DWORD,DWORD>(xPos,yPos)] += 1;
    }
}

void MainWindow::updatePresses(WPARAM wParam, LPARAM lParam)
{
    if (sessionIsRunning) {

        switch(wParam){
            case WM_KEYUP:{
                keyDown = true;
                break;
            }
            case WM_KEYDOWN:
            {
                ++totalPresses;
                ui->totalKeyPressesText->setText(QString::number(totalPresses));

                kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
                lastVkCode = kbdStruct.vkCode;
                if (kbdStruct.vkCode >= 0x41 && kbdStruct.vkCode <= 0x5A) { // Between A - Z
                    alphabetMap[kbdStruct.vkCode] += 1;
                    alphabetChart->viewport()->update();

                }
            }
        }
    }
}

void MainWindow::updateTimer()
{
    ++totalElaspedSeconds;
    QString time = QString::number((totalElaspedSeconds / 3600) % 60) + "h ";
    time += QString::number((totalElaspedSeconds / 60) % 60) + "m " ;
    time += QString::number(totalElaspedSeconds % 60) + "s  " ;
    ui->elaspedTimeText->setText(time);
    double apm = floor((double)(totalClicks+totalPresses) / (double)(totalElaspedSeconds + 1) * 60);
    ui->actionsPerMinuteText->setText(QString::number(apm));
}

void MainWindow::on_startSessionButton_clicked()
{
    sessionIsRunning = true;
    timer->start(1000);
}

void MainWindow::runConcurrentHeatmap(){
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    HeatMap heatMap(width,height,mousePosMap);
    QImage image =  heatMap.createHeatMap();
    if (ui->saveHeatmapCheckbox->isChecked()) {
        if (!image.save("C:\\Users\\white\\Pictures\\test.png")) {
           qDebug() << "Image failed to save";
         }
    }
    ui->heatmapLabel->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_createHeatMapButton_clicked()
{
    if (sessionIsRunning) {
        timer->stop();
        QFuture<void> future = QtConcurrent::run(this,&MainWindow::runConcurrentHeatmap);
    }
    sessionIsRunning = false;

}

void MainWindow::on_newSessionButton_clicked()
{
    sessionIsRunning = false;
    ui->heatmapLabel->clear();
    timer->stop();
    ui->elaspedTimeText->setText("");
    alphabetMap.clear();
    alphabetChart->viewport()->update();
    totalClicks = 0;
    totalElaspedSeconds = 0;
    totalMouseMoves = 0;
    totalPresses = 0;
    ui->totalClicksText->setText("");
    ui->actionsPerMinuteText->setText("");
    ui->totalKeyPressesText->setText("");

}
