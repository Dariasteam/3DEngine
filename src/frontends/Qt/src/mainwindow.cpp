#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  canvas = new Canvas (this);  
  this->setCentralWidget(canvas);  
}

MainWindow::~MainWindow() {
  delete ui;
}
