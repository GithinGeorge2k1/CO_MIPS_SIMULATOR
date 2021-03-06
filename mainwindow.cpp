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
    QString text=in.readAll();
    MainWindow::fun(text);
//    ui->textBrowser_2->setPlainText(text);

    //Load file Display all text();
    //means parse and store data and keep it for further stages;
}

void MainWindow::fun(QString& x){
    ui->textBrowser_2->setPlainText(x);
}
