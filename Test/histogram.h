#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
class Histogram
{
public:
    Histogram(Mat hist_h, Mat hist_s, Mat hist_v);
    ~Histogram();
    Mat hist_h;
    Mat hist_s;
    Mat hist_v;
};

#endif // HISTOGRAM_H
