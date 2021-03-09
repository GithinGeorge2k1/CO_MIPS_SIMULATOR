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
#include <QDebug>

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

bool isValue(QString R);
int convertToInt(QString R);

int storeAllLabelsAndData(QTextStream& in){
    bool h1=false,h2=false,h3=false; //.text, .data, .globl main
    //LABEL SHOULD GET CORRECT INSTRUCTION NO(NOT TO BE CONFUSED WITH LINE NO)
    int instructionsize=0;

    int start=-1;
    Data* x=Data::getInstance();
    int lineNo=0;

    while(!in.atEnd()){
        lineNo++;
        QString text=in.readLine().simplified();
        if(text[0]=='#' || text=="")
              continue;
        if(text.indexOf('#')!=-1)
            text=text.section('#',0,0);
        //ADDING DATA TO THE DATA SECTION
        if(!h1 && text==".data"){
            QRegExp sep("(,| |, )");
            bool checkingDataInNextLine=true;
            while(checkingDataInNextLine){
                lineNo++;
                text=in.readLine().simplified();
                QStringList list=text.split(sep);
                if(list.at(0).indexOf(":")==list.at(0).length()-1){
                    x->variableMap[list.at(0).section(':',0,0)]=x->dataSize;
                }
                if(text.contains(".word")){
                    checkingDataInNextLine=true;
                    for(int i=0;i<list.length();i++){
                        if(isValue(list.at(i))){
                            x->data[x->dataSize]=convertToInt(list.at(i));
                            x->dataSize++;
                        }
                    }

                }
                if(!h2 && text==".text"){
                    h2=true;
                    checkingDataInNextLine=false;
                }
            }
            h1=true;
            continue;
        }

        if(!h3 && text==".globl main"){
            h3=true;
            start=lineNo;
            continue;
        }
        QStringList list=text.split(" ");
        if(list.at(0).indexOf(":")==list.at(0).length()-1){
            x->labelMap[list.at(0).section(':',0,0)]=instructionsize;
        }
        if(list.length()!=1)
            instructionsize++;

    }
    return start;
}

void MainWindow::on_actionReinitialize_and_Load_File_triggered()
{
    int start=-1;
    Data* x=Data::getInstance();
    x->initialize();
    QString path=QFileDialog::getOpenFileName(this,"title");
    QFile file(path);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,"title","File not Opened");
        return;
    }
    QTextStream in(&file);
    ui->textBrowser_2->setPlainText("");
    ui->textBrowser_3->setPlainText("");
    bool lineValid;
    int lineNo=0;
    start=storeAllLabelsAndData(in);
    in.seek(0);
    QString dataText=x->displayData();
    ui->textBrowser_3->setPlainText(dataText);
    while(!in.atEnd()){
        lineNo++;
        QString text=in.readLine().simplified();
        if(lineNo<=start){
            continue;
        }
        if(text[0]=='#' || text=="")
              continue;
        if(text.indexOf('#')!=-1){
            text=text.section('#',0,0);
        }
        lineValid=x->addCode(text);
        if(lineValid){
            ui->textBrowser_2->append(QString("%1").arg(text));
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

void MainWindow::on_actionSee_LabelMap_triggered()
{
    Data *D=Data::getInstance();
    qDebug()<<D->labelMap;

}

void MainWindow::on_actionSee_DataMap_triggered()
{
    Data *D=Data::getInstance();
    qDebug()<<D->variableMap;

}
