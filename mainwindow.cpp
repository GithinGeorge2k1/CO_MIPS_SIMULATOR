#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QStringList>
#include "Data.h"
#include "Maps.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), ValidCodePresent(false)
{
    ui->setupUi(this);
    refreshRegisterPanel();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::refreshRegisterPanel(){
    Data* x=Data::getInstance();
    QString text=x->displayRegisters();
    ui->textBrowser->setPlainText(text);
}

void MainWindow::on_actionReinitialize_and_Load_File_triggered()
{
    Data* x=Data::getInstance();
    bool h1=false,h2=false,h3=false; //.text, .data, .globl main
    QString path=QFileDialog::getOpenFileName(this,"title");
    QFile file(path);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,"title","File not Opened");
        return;
    }
    QTextStream in(&file);
    ui->textBrowser_2->setPlainText("");
    bool lineValid;
    int lineNo=0;
    while(!in.atEnd()){
        lineNo++;
        QString text=in.readLine().simplified();
        //remove starting commands and empty file
        if(text[0]=='#' || text=="")
              continue;
        //remove comments in between
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
        lineValid=x->addCode(text);
        if(lineValid){
            ui->textBrowser_2->append(text);
            MainWindow::ValidCodePresent=true;
        }
        else{
            QMessageBox::warning(this,"Invalid Assembly Code",QString("Syntax error found while parsing at line %1").arg(lineNo));
            MainWindow::ValidCodePresent=false;
            return;
        }
    }
    file.close();

}

void MainWindow::on_actionQuit_triggered()
{
    delete Data::getInstance();
    QApplication::quit();
}

void MainWindow::on_actionInitialize_triggered()
{
    Data* x=Data::getInstance();
    ui->textBrowser_2->setPlainText("");
    ui->textBrowser_3->setPlainText("");
    x->initialize();
    ui->textBrowser->setPlainText(x->displayRegisters());
}
