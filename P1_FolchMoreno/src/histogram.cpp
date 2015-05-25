#include "histogram.h"

/**
  *
  * Aquesta classe conté la informació necessaria per a un histograma.
  */

Histogram::Histogram(Mat hist_h, Mat hist_s, Mat hist_v) {
    this->hist_h = new Mat(hist_h);
    this->hist_s = new Mat(hist_s);
    this->hist_v = new Mat(hist_v);
}

/**
  * Compara un histograma amb un altre amb el mètode escollit
  *
  */

double Histogram::compare(Histogram *h) {
    int compare_method;

    if (METHOD_COMPARE == 1)
        compare_method = CV_COMP_CORREL; // Correlation
    else if (METHOD_COMPARE == 2)
        compare_method = CV_COMP_CHISQR; // Chi-Square
    else if (METHOD_COMPARE == 3)
        compare_method = CV_COMP_INTERSECT; // Intersection
    else if (METHOD_COMPARE == 4)
        compare_method = CV_COMP_BHATTACHARYYA; // Bhattacharyya distance

    double comphist_h = compareHist( *hist_h, *h->hist_h, compare_method);
    double comphist_s = compareHist( *hist_s, *h->hist_s, compare_method);
    double comphist_v = compareHist( *hist_v, *h->hist_v, compare_method);

    return comphist_h + comphist_s + comphist_v;
}

Histogram::~Histogram()
{

}

