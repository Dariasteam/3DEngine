#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <chrono>
#include <thread>

#include "canvas.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  inline Canvas* get_canvas () { return canvas; }
private:
  Canvas* canvas;    

  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
