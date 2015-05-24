#include "mainwindow.h"


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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImages(){

    QList<QString> *images = controller->getFilesDirectory(getDirectoryPath());
    controller->insertImages(images);

}


void MainWindow::exit(){
    std::exit(0);
}
void MainWindow::chooseImage(){
    QString path = MainWindow::getFile();
    if(path.compare("") == 0)
        return;
    ui->imageinput->setText(path);
    QList<QString> *list = controller->search(path);
    showImages(list);
}

QString MainWindow::getFile(){
    return QFileDialog::getOpenFileName(this,tr("Directory"),NULL,tr("All images (*.jpg *.jpeg *.png);;JPEG (*.jpg *.jpeg);;PNG (*.png)" ));
}

/**
 * Returns a selected directory path.
 *
 * @brief MainWindow::getDirectoryPath
 * @return
 */

QString MainWindow::getDirectoryPath() {
    //return "/home/afolchga8.alumnes/openmp-project/Test-build-desktop-Qt_4_8_2_in_PATH__System__Debug/images";
    return QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                             "/home",
                                             QFileDialog::ShowDirsOnly
                                             | QFileDialog::DontResolveSymlinks);

}



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


















