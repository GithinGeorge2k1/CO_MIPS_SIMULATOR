#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionReinitialize_and_Load_File_triggered()
{

    QString path=QFileDialog::getOpenFileName(this,"title");
    QFile file(path);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        return;
    }
    QTextStream in(&file);
    int i=0;
    while(!in.atEnd()){
        QString text=in.readLine(i);
        ui->textBrowser_2->append(text + "BLAH");
    }
    file.close();
}
