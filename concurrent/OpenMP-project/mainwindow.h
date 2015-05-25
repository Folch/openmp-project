#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include <omp.h>
#include <QWidget>
#include <QPushButton>
#include <QApplication>
#include <QGridLayout>
#include <QFileInfo>
#include <QLabel>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "controller.h"
#include "ui_mainwindow.h"
#include <iostream>


using namespace std;
using namespace cv;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString getDirectoryPath();
    QString getFile();
    void showImages(QList<QString>*);

private:
    Ui::MainWindow *ui;
    Controller *controller;


public slots:
    void loadImages();
    void exit();
    void chooseImage();

};

#endif // MAINWINDOW_H
