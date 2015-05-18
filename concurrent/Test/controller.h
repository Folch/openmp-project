#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <qstring.h>
#include <QList>
#include <dirent.h>
#include "histogram.h"
#include <sys/stat.h>

#include <math.h>
#include <iostream>

using namespace std;


#include <stdlib.h>
#include <iostream>
#include <QStringList>
#include <quicksort.h>

#define IMG_PATH "./img/"
#define HIST_PATH "./hist/"
#define XNUMBER 6

using namespace std;
class Controller
{
public:

    Controller();
    ~Controller();
    QList<QString> *search(QString path);
    QList<QString> *getFilesDirectory(QString path);

    void loadHist(QList<QString> *list);

    void insertImages(QList<QString> *list);

private:
    Histogram **histograms;
    int id;
};

#endif // CONTROLLER_H
