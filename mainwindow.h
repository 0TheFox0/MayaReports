#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QMessageBox>
#include <QPrintDialog>
#include <QPrinter>
#include <QFile>
#include <QTime>
#include <QProgressDialog>
#include <QApplication>
#include <QThread>
#include "paper.h"
#include "reportview.h"
#include "reportrenderer.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:

    void element_toggled(bool arg1);

    void on_actionGuardar_triggered();

    void on_actionAbrir_triggered();

    void stopInsert();
    void on_actionBorrar_triggered();

    void on_actionConfigurar_pagina_triggered();

    void on_actionVista_Previa_triggered();

    void previewReady();
    void on_actionBlock_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    Paper paper;
    ReportView view;
    QAction* insertor;
    ReportRenderer* render;
};

#endif // MAINWINDOW_H
