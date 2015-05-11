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

QList<QString> Controller::search(QString path) {

}

void Controller::loadHist(QList<QString> list) {

}

/**
 * Returns a set of files given a directory path.
 *
 * @brief MainWindow::getFilesDirectory
 * @return
 */

QList<QString> Controller::getFilesDirectory(QString path) {

}

Controller::~Controller()
{

}

