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
    render = 0;
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
    gr.setDatabaseName("grupotestt");
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
    if(render)
        delete render;
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
    render = new ReportRenderer();

    render->setPrinter(printer);
    render->setDocIn(doc);
    QMap<QString,QString> c;
    c["Empresa.cab_fac"] = "id = 1";
    c["Empresa.lin_fac"] = "id_cab = 1";
    render->setQueryClausules(c);

    QtConcurrent::run(render, &ReportRenderer::PreRender);
    connect(render,SIGNAL(end()),pDlg,SLOT(deleteLater()));
    connect(render,SIGNAL(end()),this,SLOT(previewReady()));
}

void MainWindow::previewReady()
{
    QPrintPreviewDialog * pre = new QPrintPreviewDialog(render->getPrinter(),this);
    connect(pre, SIGNAL(paintRequested(QPrinter*)), render, SLOT(Print(QPrinter*)));
    pre->show();
}

void MainWindow::on_actionBlock_triggered()
{
    QList<QGraphicsItem*> l =  scene.selectedItems();
    for(int i = 0;i< l.size(); i++)
    {
       Container* c = qgraphicsitem_cast<Container*>(l.at(i));
       if(c)
           c->setActive(false);
    }
}
