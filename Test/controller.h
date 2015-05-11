#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <qstring.h>
#include <QList>
class Controller
{
public:
    const QString IMG_PATH = "./img/";
    const QString HIST_PATH = "./hist/";


    Controller();
    ~Controller();
    QList<QString> search(QString path);
    QList<QString> getFilesDirectory(QString path);
    void loadHist(QList<QString> list);
    void insertImages(QList<QString> list);

private:
    //QList<histogram> histograms;
};

#endif // CONTROLLER_H
