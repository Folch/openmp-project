#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <qstring.h>
class Controller
{
public:
    const QString IMG_PATH = "./img/";
    const QString HIST_PATH = "./hist/";


    Controller();
    ~Controller();
    void search();
    void loadHist(QList<QString> list);
    void insertImages(QList<QString> list);

private:
    //QList<histogram> histograms;
};

#endif // CONTROLLER_H
