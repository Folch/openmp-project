#include "controller.h"

QString IdToString(int id) {
    int len = log10(id);
    len++;
    len = XNUMBER - len;
    QString out;
    for (int i = 0; i < len; ++i)
        out.append("0");

    return out+QString::number(id);
}

Controller::Controller() {
    /*Cridar a loadHist per carregar els histogrames*/
    id=1;
    QList<QString> *list = getFilesDirectory(HIST_PATH);
    loadHist(list);
}



Histogram* getHistogram(int id) {

    Mat hist_h, hist_s, hist_v;
    QString path = "hist_"+IdToString(id);
    FileStorage fs(path.toLatin1().data(), FileStorage::READ);

    fs ["hist_h"] >> hist_h;
    fs ["hist_s"] >> hist_s;
    fs ["hist_v"] >> hist_v;

    return new Histogram(hist_h, hist_s, hist_v);
}


Histogram* createHistogram(QString path) {

}

void storeHistogram(int id, Histogram* h) {
    QString path = QString(HIST_PATH) + QString("hist_") + IdToString(id) + ".xml";
    FileStorage fs(path.toLatin1().data(), FileStorage::WRITE);

    fs << "hist_h" << h->hist_h;
    fs << "hist_s" << h->hist_s;
    fs << "hist_v" << h->hist_v;

    fs.release();
}

void Controller::insertImages(QList<QString> *list) {

    /*Hem de crear la carpeta 'img/' i 'hist/' si no està creada*/
    DIR *imgdir = opendir(IMG_PATH);
    DIR *histdir = opendir(HIST_PATH);

    if(imgdir == NULL){
        mkdir(IMG_PATH,ACCESSPERMS);
        imgdir = opendir(IMG_PATH);
    }

    if(histdir == NULL){
        mkdir(HIST_PATH,ACCESSPERMS);
        histdir = opendir(HIST_PATH);
    }

    int i;
    for(i = 0; i < list->size(); i++){
        QStringList splitted = list->at(i).split('/');
        QString name = splitted.last();
        std::cout<< list->at(i).toLatin1().data();
        QString from = "cp "+list->at(i)+" "+IMG_PATH + name;
        //std::cout<< from.toLatin1().data();
        system (from.toLatin1().data());
        Histogram *histogram = createHistogram(list->at(i));
        histograms->append(histogram);


        id++;

    }
    /*Carregar cada imatge
            * Copiar la imatge a la carpeta 'img/'
            * Calcular l'histograma
            * Guardar l'histograma a 'hist/'
    */


}

QList<QString> *Controller::search(QString path) {

}

void Controller::loadHist(QList<QString> *list) {
    histograms = new QList<Histogram*>;
    for (id = 1; id <= list->size(); ++id) {
        histograms->append(getHistogram(id));
    }
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


          if(epdf->d_name[0] == '.')
              continue;
           std::cout << epdf->d_name << std::endl;
          images->append(path+"/"+ QString(QLatin1String(epdf->d_name)));

       }
    }
    return images;
}

Controller::~Controller()
{

}

