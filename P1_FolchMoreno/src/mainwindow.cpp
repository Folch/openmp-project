#include "mainwindow.h"

/**
 * Constructor del MainWindow, es vinculen les senyals dels butons de la gui amb les funcions.
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this->ui->actionLoad_Image,SIGNAL(triggered()),this,SLOT(loadImages()));
    connect(this->ui->actionExit,SIGNAL(triggered()),this,SLOT(exit()));
    connect(this->ui->chooseDirectory,SIGNAL(clicked()),this,SLOT(chooseImage()));

    controller = new Controller();
}
/**
 * Destructor de la classe.
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * Carrega les imatges del path escollit per l'usuari i les guarda la carpeta pròpia.
 * @brief MainWindow::loadImages
 */
void MainWindow::loadImages(){

    QList<QString> *images = controller->getFilesDirectory(getDirectoryPath());
    controller->insertImages(images);

}

/**
 * Funció que es crida al fer click a la opció de tancar.
 * @brief MainWindow::exit
 */
void MainWindow::exit(){
    std::exit(0);
}

/**
 * Funció que li demana a l'usuari la imatge a comparar amb la base de dades, fa la cerca i mostra les 9 imatges més semblants.
 * @brief MainWindow::chooseImage
 */
void MainWindow::chooseImage(){
    QString path = MainWindow::getFile();
    if(path.compare("") == 0)
        return;
    ui->imageinput->setText(path);
    QList<QString> *list = controller->search(path);
    showImages(list);
}
/**
 * Funció que es crida per seleccionar una imatge per cercar en format jpg.
 * @brief MainWindow::getFile
 * @return
 */
QString MainWindow::getFile(){
    return QFileDialog::getOpenFileName(this,tr("Directory"),NULL,tr("JPEG (*.jpg *.jpeg);;" ));
}

/**
 * Retorna el path a la carpeta on estan les imatges per crear la base de dades.
 *
 * @brief MainWindow::getDirectoryPath
 * @return
 */

QString MainWindow::getDirectoryPath() {
    return QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                             "/home",
                                             QFileDialog::ShowDirsOnly
                                             | QFileDialog::DontResolveSymlinks);

}


/**
 * Funció que mostra en 3x3 les imatges més semblants al filesList.
 * @brief MainWindow::showImages
 * @param filesList
 */
void MainWindow::showImages(QList<QString> *filesList){
    QWidget *imagesWidget = new QWidget();
    QGridLayout *grid = ui->gridLayout;

    QImage copy;
    QString tempFileName;
    QList<QImage> images;
    /*QList<QString> filesList;
    filesList << "image1.png" << "image2.png" << "image3.png";*/
    Mat img;
    int n = 3;
    for (int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            tempFileName = QFileInfo(filesList->at(i*n+j)).absoluteFilePath();
            img = imread(tempFileName.toStdString(), CV_LOAD_IMAGE_COLOR  );
            cvtColor(img, img, CV_BGR2RGB);
            QImage qimg = QImage((uchar*) img.data, img.cols, img.rows,
                                  img.step, QImage::Format_RGB888);
            //QImage image(tempFileName);
            qimg = qimg.scaled(200,200,Qt::KeepAspectRatio);
            QPixmap p = QPixmap::fromImage(qimg);
            //QPixmap p = QPixmap::fromImage(images[i*n+j]);
            QLabel *label = new QLabel(imagesWidget);
            label->setPixmap(p);
            grid->addWidget(label, i, j);
        }
    }
}


















