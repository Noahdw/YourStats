#ifndef HEATMAP_H
#define HEATMAP_H

#include <map>
#include <vector>
#include <Windows.h>
#include <QImage>
#include <QDebug>
class HeatMap
{
public:
    HeatMap(int _width, int _height, const std::map <std::pair<DWORD,DWORD>,int> &mousePosMap);
    QImage createHeatMap();
private:
    int width = 0;
    int height = 0;
    const  std::map <std::pair<DWORD,DWORD>,int> *posMap;
    std::vector<std::vector<int>> matrix;
    int highestValue = 0;
    int radius = 50;
    const int NUM_COLORS = 4;
    const float color[4][3] = { {0,0,255}, {0,255,0}, {255,255,0}, {255,0,0} };
};

#endif // HEATMAP_H
