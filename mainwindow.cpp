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
    bool h1=false,h2=false,h3=false;
    QString path=QFileDialog::getOpenFileName(this,"title");
    QFile file(path);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        return;
    } 
    QTextStream in(&file);
//    int i=0;
    ui->textBrowser_2->setPlainText("");
    while(!in.atEnd()){
        QString text=in.readLine().simplified();
        if(text[0]=='#' || text=="")
              continue;
        if(text.indexOf('#')!=-1){
            text=text.section('#',0,0);
        }
        if(!h1 && text==".data"){
            //set all data words....... here and now

            h1=true;
            continue;
        }
        if(!h2 && text==".text"){
            h2=true;
            continue;
        }
        if(!h3 && text==".globl main"){
            h3=true;
            continue;
        }

        ui->textBrowser_2->append(text);
    }
    file.close();
}
