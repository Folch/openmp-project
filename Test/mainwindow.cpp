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

    QList<QString> *images = getFilesDirectory(getDirectoryPath());
    controller->insertImages(*images);

}
QList<QString>* MainWindow::getFilesDirectory(QString path){
    DIR *dpdf;
    struct dirent *epdf;
    QList<QString> *images = new QList<QString>();
    //dpdf = opendir("./");
    dpdf = opendir(path.toLatin1().data());
    if (dpdf != NULL){
       while ((epdf = readdir(dpdf))){

           //cout<<"Filename: "<< epdf->d_name;
          std::cout << epdf->d_name << std::endl;
          images->append( QString(QLatin1String(epdf->d_name)));
       }
    }
    return images;
}

void MainWindow::exit(){
    std::exit(0);
}
void MainWindow::chooseImage(){
    QString path = MainWindow::getFile();
    ui->imageinput->setText(path);
    cout <<  path.toStdString() << endl;

}

QString MainWindow::getFile(){
    return QFileDialog::getOpenFileName(this,tr("Directory"),NULL,tr("All files (*.*);;JPEG (*.jpg *.jpeg);;PNG (*.png)" ));
}

/**
 * Returns a selected directory path.
 *
 * @brief MainWindow::getDirectoryPath
 * @return
 */

QString MainWindow::getDirectoryPath() {
    return "/home/afolchga8.alumnes/openmp-project/Test";
}



void MainWindow::showImages(QList<QString> filesList){
    QWidget *imagesWidget = new QWidget();
    QGridLayout *grid = ui->gridLayout;

    QImage copy;
    QString tempFileName;
    QList<QImage> images;
    /*QList<QString> filesList;
    filesList << "image1.png" << "image2.png" << "image3.png";*/

    foreach(QFileInfo fileInfo, filesList){

        tempFileName = fileInfo.absoluteFilePath();
        cout << tempFileName.toStdString() << endl;
        Mat image2 = imread(tempFileName.toStdString(), CV_LOAD_IMAGE_COLOR);
        QImage image = QImage((uchar*) image2.data, image2.cols, image2.rows, image2.step, QImage::Format_RGB888);
        //QImage image(tempFileName);
        copy = image.scaled(200,200,Qt::KeepAspectRatio);
        images.append(copy);
    }
    int n = 3;
    for (int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            QPixmap p(QPixmap::fromImage(images[i]));
            QLabel *label = new QLabel(imagesWidget);
            label->setPixmap(p);
            grid->addWidget(label, i, j);
        }
    }
}
