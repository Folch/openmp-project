#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <qstring.h>
#include <QList>
#include <dirent.h>
#include "histogram.h"
#include <sys/stat.h>
#include <time.h>

#include <math.h>
#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <sys/sendfile.h>  // sendfile
#include <fcntl.h>         // open
#include <unistd.h>        // close
#include <sys/stat.h>      // fstat
#include <sys/types.h>     // fstat
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
