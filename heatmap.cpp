#include "heatmap.h"

HeatMap::HeatMap(int _width, int _height, const std::map <std::pair<DWORD,DWORD>,int> &mousePosMap)
    : width(_width), height(_height)
{
    posMap = &mousePosMap;
}

QImage HeatMap::createHeatMap()
{
    //std::vector<std::vector<float>> copyMatrix;
    //Initialize the matrix
    matrix.resize(width);
    //copyMatrix.resize(width);
    for(int i = 0 ; i < width ; ++i)
    {
        matrix[i].resize(height,0);
        //copyMatrix[i].resize(height,0.0f);
    }

    //Constuct a circle with the given radius around each data point
    // and convert the data point map to an array
    for (auto var : *posMap) {
        if(var.first.first <= width && var.first.second <= height){
            matrix[var.first.first][var.first.second] += var.second;
            int xPos = var.first.first;
            int yPos = var.first.second;
            for (int x = xPos - radius; x < xPos + radius; ++x) {
                for (int y = yPos - radius; y < yPos + radius; ++y) {

                    if (((x - xPos) * (x - xPos) + (y - yPos) * (y - yPos)) <= radius*radius) {
                        if(x >= 0 && x  < width && y >= 0 && y < height){
                            matrix[x][y] += 1;
                        }

                    }
                }
            }

        }

    }
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if(matrix[x][y] > highestValue){
                highestValue = matrix[x][y];
            }
        }
    }

    // normalize data
    //    for (int x = 0; x < width; ++x) {
    //        for (int y = 0; y < height; ++y) {
    //            int avg = 0;
    //            int n = 0;
    //            if(matrix[x][y] > highestValue){
    //                highestValue = matrix[x][y];
    //            }
    //            avg += matrix[x][y];
    //            ++n;
    //            if (x > 0) {
    //                avg +=  matrix[x-1][y];
    //                ++n;
    //            }
    //            if (x < width - 1) {
    //                avg +=  matrix[x+1][y];
    //                ++n;
    //            }
    //            if (y > 0) {
    //                avg +=  matrix[x][y-1];
    //                ++n;
    //            }
    //            if (y < height - 1) {
    //                avg +=  matrix[x][y+1];
    //                ++n;
    //            }
    //            if (x > 0 && y > 0) {
    //                avg +=  matrix[x-1][y -1];
    //                ++n;
    //            }
    //            if (x > 0 && y < height - 1) {
    //                avg +=  matrix[x-1][y +1];
    //                ++n;
    //            }
    //            if (x < width - 1 && y > 0) {
    //                avg +=  matrix[x+1][y -1];
    //                ++n;
    //            }
    //            if (x < width- 1 && y < height- 1) {
    //                avg +=  matrix[x+1][y +1];
    //                ++n;
    //            }
    //            float favg = (float)avg /8;
    //            copyMatrix[x][y] = favg;
    //        }
    //    }
    QImage image(width,height,QImage::Format_RGB32);
    float fvalue;
    QRgb value;

    //http://www.andrewnoske.com/wiki/Code_-_heatmaps_and_color_gradients
    //This was used for the actual heatmap logic

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            fvalue = matrix[x][y] / (float)highestValue;

            // A static array of 4 colors:  (blue,   green,  yellow,  red) using {r,g,b} for each.

            int idx1;        // |-- Our desired color will be between these two indexes in "color".
            int idx2;        // |
            float fractBetween = 0;  // Fraction between "idx1" and "idx2" where our value is.

            if(fvalue <= 0)      {  idx1 = idx2 = 0;            }    // accounts for an input <=0
            else if(fvalue >= 1)  {  idx1 = idx2 = NUM_COLORS-1; }    // accounts for an input >=0
            else
            {
                fvalue = fvalue * (NUM_COLORS-1);        // Will multiply value by 3.
                idx1  = floor(fvalue);                  // Our desired color will be after this index.
                idx2  = idx1+1;                        // ... and before this index (inclusive).
                fractBetween = fvalue - float(idx1);    // Distance between the two indexes (0-1).
            }

            int red   = (color[idx2][0] - color[idx1][0])*fractBetween + color[idx1][0];
            int green = (color[idx2][1] - color[idx1][1])*fractBetween + color[idx1][1];
            int blue  = (color[idx2][2] - color[idx1][2])*fractBetween + color[idx1][2];

            value = qRgb(red, green, blue);
            image.setPixel(x,y,value);
        }
    }
    return image;
}
