#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QStringList>
#include "Data.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), ValidCodePresent(false)
{
    ui->setupUi(this);
    refreshRegisterPanel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshRegisterPanel(){
    Data x;
    QString text=x.displayRegisters();
    ui->textBrowser->setPlainText(text);

}
void MainWindow::on_actionReinitialize_and_Load_File_triggered()
{
    Data x;
    bool h1=false,h2=false,h3=false;
    QString path=QFileDialog::getOpenFileName(this,"title");
    QFile file(path);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        return;
    }
    QTextStream in(&file);
    ui->textBrowser_2->setPlainText("");
    bool lineValid;
    int lineNo=0;
    while(!in.atEnd()){
        lineNo++;
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
        lineValid=x.addCode(text);
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
    QApplication::quit();
}
