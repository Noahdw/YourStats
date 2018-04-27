#include "barchart.h"

BarChart::BarChart(QWidget *parent) : QGraphicsView(parent)
{

}

BarChart::BarChart(const std::map<DWORD, int> &map, const int &strokes, QWidget *parent) : QGraphicsView(parent)
{
    totalStrokes = &strokes;
    barMap = &map;
    scene = new QGraphicsScene;
    scene->setSceneRect(0,0,200,100);
    setScene(scene);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMinimumHeight(100);
    setMaximumHeight(100);

   setViewportUpdateMode(FullViewportUpdate);
   scale(1,2);
}



void BarChart::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this->viewport());
    QPen pen;
    // pen.setWidthF(0.5);
    pen.setColor(Qt::black);
    painter->setBrush(Qt::lightGray);
    painter->setPen(pen);
    int highest = 0;
    for(auto &var : *barMap) {
        if (var.second > highest) {
            highest = var.second;
        }
    }
    ++highest;
    auto item = barMap->begin();
    for (int bar = 0; bar < barMap->size(); ++bar) {
        int OldRange = (highest - 0);
        int NewRange = (height() - 0);
        int NewValue = (((item->second - 0) * NewRange) / OldRange) + 0;
        painter->drawRect(bar*width()/barMap->size(),height()-NewValue,width()/barMap->size(),NewValue);
        ++item;
    }

    QGraphicsView::paintEvent(event);
}
