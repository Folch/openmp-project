#include "controller.h"

Controller::Controller() {
    /*Cridar a loadHist per carregar els histogrames*/
    id=1;
    //loadHist(list);
}

QString IdToString(int id) {

}

Histogram* getHistogram(int id) {

}

Histogram* createHistogram(QString path) {

}

void Controller::insertImages(QList<QString>* list) {
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

void Controller::loadHist(QList<QString> list) {

}

/**
 * Returns a set of files given a directory path.
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

