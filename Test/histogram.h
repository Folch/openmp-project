#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define METHOD_COMPARE 3

using namespace cv;
class Histogram
{
public:
    Histogram(Mat hist_h, Mat hist_s, Mat hist_v);
    ~Histogram();
    Mat hist_h;
    Mat hist_s;
    Mat hist_v;

    double compare(Histogram *h);
};

#endif // HISTOGRAM_H
