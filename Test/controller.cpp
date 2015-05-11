#include "controller.h"

Controller::Controller() {
    /*Cridar a loadHist per carregar els histogrames*/

    //loadHist(list);
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
          //std::cout << epdf->d_name << std::endl;
          images->append( QString(QLatin1String(epdf->d_name)));
       }
    }
    return images;
}

Controller::~Controller()
{

}

