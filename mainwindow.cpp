#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include "editpaperdlg.h"
#include <QtConcurrent/QtConcurrent>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(this),
    view(this)
{
    ui->setupUi(this);
    view.setScene(&scene);
    this->setCentralWidget(&view);

    QFontDatabase::addApplicationFont(":/codeBar/codebar/FRE3OF9X.TTF");
    //Free 3 of 9 Extended

    //TODO cambiar dentro de Maya
    QSqlDatabase gr = QSqlDatabase::addDatabase("QMYSQL","grupo");
    gr.setHostName("localhost");
    gr.setUserName("root");
    gr.setPassword("marco");
    gr.setDatabaseName("grupotesting");
    gr.open();
    QSqlDatabase em = QSqlDatabase::addDatabase("QMYSQL","empresa");
    em.setHostName("localhost");
    em.setUserName("root");
    em.setPassword("marco");
    em.setDatabaseName("empsarapin");
    em.open();

    scene.setBackgroundBrush(QBrush(Qt::gray));

    scene.addItem(&paper);
    paper.setSize(Paper::A4);
    paper.setOrientacion(Paper::Retrato);
    paper.setMargen(1.5);

    connect(&paper,SIGNAL(itemInserted()),this,SLOT(stopInsert()));

    connect(ui->actionRectangulo_redondeado,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionTexto,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));  
    connect(ui->actionLinea,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionCodeBars,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionImagen,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionCampo,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionCampo_Relacional,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));   
}

MainWindow::~MainWindow()
{
    delete ui;
    render->deleteLater();
}

void MainWindow::element_toggled(bool arg1)
{
    if(arg1)
    {
        QObject * send = sender();
        insertor = qobject_cast<QAction*>(send);
        if(send == ui->actionRectangulo_redondeado)
            paper.prepareItemInsert(Paper::RoundRectIt);
        else if(send == ui->actionTexto)
            paper.prepareItemInsert(Paper::Label);
        else if(send == ui->actionLinea)
            paper.prepareItemInsert(Paper::Linea);
        else if(send == ui->actionCodeBars)
            paper.prepareItemInsert(Paper::CodeBarIt);
        else if(send == ui->actionImagen)
            paper.prepareItemInsert(Paper::Imagen);
        else if(send == ui->actionCampo)
            paper.prepareItemInsert(Paper::Campo);
        else if(send == ui->actionCampo_Relacional)
            paper.prepareItemInsert(Paper::CampoRelacional);
    }
    else
        paper.stopInsertingItems();
}

void MainWindow::on_actionGuardar_triggered()
{
    QString file = QFileDialog::getSaveFileName(qApp->activeWindow(),tr("Guardar Report"),QString(),"XML (*.xml)");
    int i = paper.save(file);
    if(i<0)
    {
        if (i== -1)
            QMessageBox::critical(this,"Error","Error al abrir el archivo");
        else
            QMessageBox::warning(this,"Cuidado","Tiene elementos fuera de secciones.\nEstos no han sido guardados.");
    }
}

void MainWindow::on_actionAbrir_triggered()
{
    QString fil = QFileDialog::getOpenFileName(this,"Abir report",QString(),"XML (*.xml)");

    if(fil.isEmpty())
        return;

    QFile ff(fil);
    QString error;
    if(ff.open(QFile::ReadOnly))
    {
        QString x = ff.readAll();
        if(!paper.parseXML(x,error))
            QMessageBox::critical(this,"Error",error);
    }
    ff.close();
}

void MainWindow::stopInsert()
{
    insertor->setChecked(false);
}

void MainWindow::on_actionBorrar_triggered()
{
    paper.removeItems(scene.selectedItems());
}

void MainWindow::on_actionConfigurar_pagina_triggered()
{
    EditPaperDlg dlg(&paper,this);
    dlg.exec();
}

void MainWindow::on_actionVista_Previa_triggered()
{
    QProgressDialog* pDlg = new QProgressDialog(this);
    pDlg->setMaximum(0);
    pDlg->setValue(0);
    pDlg->show();

    QPrinter * printer = new QPrinter();
    QPrintDialog dlg(printer,this);
  //  bool error;

    QDomDocument doc = paper.preview();
    QThread * thread = new QThread(this);
    render = new ReportRenderer();

    render->setPrinter(printer);
    render->setDocIn(doc);

  //  render->moveToThread(thread);
    QtConcurrent::run(render, &ReportRenderer::PreRender);
  //  connect(thread,SIGNAL(started()),render,SLOT(PreRender()));
  //  connect(render, SIGNAL(end()), thread, SLOT(quit()));
//    connect(thread,SIGNAL(finished()),pDlg,SLOT(deleteLater()));
//    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
//    connect(thread,SIGNAL(finished()),this,SLOT(previewReady()));
//    thread->start();
    connect(render,SIGNAL(end()),pDlg,SLOT(deleteLater()));
    connect(render,SIGNAL(end()),this,SLOT(previewReady()));
}

void MainWindow::previewReady()
{
    QPrintPreviewDialog * pre = new QPrintPreviewDialog(render->getPrinter(),this);
    connect(pre, SIGNAL(paintRequested(QPrinter*)), render, SLOT(Print(QPrinter*)));
    pre->show();
}
