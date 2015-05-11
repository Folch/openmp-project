#include "histogram.h"

Histogram::Histogram(Mat hist_h, Mat hist_s, Mat hist_v) {
    this->hist_h = hist_h;
    this->hist_s = hist_s;
    this->hist_v = hist_v;
}

Histogram::~Histogram()
{

}

