#ifndef BARCHART_H
#define BARCHART_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <Windows.h>
#include <map>


class BarChart : public QGraphicsView
{
    Q_OBJECT
public:
     BarChart(QWidget *parent = nullptr);
     BarChart(const std::map<DWORD,int> &map,const int &strokes,QWidget *parent = nullptr);

private:
     QGraphicsScene * scene;
     const std::map<DWORD,int> *barMap;
     const int *totalStrokes;
protected:
     void paintEvent(QPaintEvent *event);
signals:

public slots:
};

#endif // BARCHART_H
