#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <qstring.h>
#include <QList>
#include <dirent.h>
#include "histogram.h"
#include <math.h>
#include <iostream>

using namespace std;

#define IMG_PATH "./img/"
#define HIST_PATH "./hist/"
#define XNUMBER 6;

class Controller
{
public:

    Controller();
    ~Controller();
    QList<QString> *search(QString path);
    QList<QString> *getFilesDirectory(QString path);
    void loadHist(QList<QString> list);
    void insertImages(QList<QString> list);

private:
    QList<Histogram*> *histograms;
    QList<QString> *imagesPaths;
    int id;
};

#endif // CONTROLLER_H
