#include "controller.h"

QString IdToString(int id) {
    int len = log10(id);
    len++;
    QString out;
    len = XNUMBER - len;
    for (int i = 0; i < len; ++i)
        out.append("0");

    return out+QString::number(id);
}

Controller::Controller() {
    /*Cridar a loadHist per carregar els histogrames*/
    id=1;
    //loadHist(list);
}



Histogram* getHistogram(int id) {

    Mat hist_h, hist_s, hist_v;
    QString path = "hist_"+IdToString(id);
    FileStorage fs(path.toLatin1().toStdString(), FileStorage::READ);

    fs ["hist_h"] >> hist_h;
    fs ["hist_s"] >> hist_s;
    fs ["hist_v"] >> hist_v;

    return new Histogram(hist_h, hist_s, hist_v);
}

void storeHistogram(int id, Histogram* h) {
    QString path = HIST_PATH + "hist_" + IdToString(id) + ".xml";
    FileStorage fs(path.toLatin1().toStdString(), FileStorage::WRITE);

    fs << "hist_h" << h->hist_h;
    fs << "hist_s" << h->hist_s;
    fs << "hist_v" << h->hist_v;

    fs.release();
}

void Controller::insertImages(QList<QString> list) {
    /*Hem de crear la carpeta 'img/' i 'hist/' si no està creada*/


    /*Carregar cada imatge
            * Copiar la imatge a la carpeta 'img/'
            * Calcular l'histograma
            * Guardar l'histograma a 'hist/'
    */


}

QList<QString> *Controller::search(QString path) {

}

void Controller::loadHist(QList<QString> list) {

}

/**
 * Returns a set of absolute path of files given a directory path.
 *
 * @brief MainWindow::getFilesDirectory
 * @return
 */

QList<QString>* Controller::getFilesDirectory(QString path){
    DIR *dpdf;
    struct dirent *epdf;
    QList<QString> *images = new QList<QString>();
    //dpdf = opendir("./");
    dpdf = opendir(path.toLatin1().data());
    if (dpdf != NULL){
       while ((epdf = readdir(dpdf))){
          //std::cout << epdf->d_name << std::endl;
          images->append( path+"/"+QString(QLatin1String(epdf->d_name)));
       }
    }
    return images;
}

Controller::~Controller()
{

}

