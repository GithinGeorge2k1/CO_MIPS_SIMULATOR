#include "cacheconfig.h"
#include "ui_cacheconfig.h"
#include "Data.h"
#include <QDebug>
bool isPowerOfTwo(int x){
    if(x<=0){
        return false;
    }
    while(x!=1){
        if(x%2!=0){
            return false;
        }
        x=x/2;
    }
    return true;
}

CacheConfig::CacheConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CacheConfig)
{
    ui->setupUi(this);
}

CacheConfig::~CacheConfig()
{
    delete ui;
}

void CacheConfig::on_pushButton_clicked()
{
    bool one=false,two=false,three=false;
    int cs=ui->lineEdit->text().toInt(&one);
    int bs=ui->lineEdit_2->text().toInt(&two);
    int assoc=ui->lineEdit_3->text().toInt(&three);
    qDebug()<<"reached this point"<<cs<<" "<<bs<< " "<< assoc << "\n";
    if(one && two && three && isPowerOfTwo(cs) && isPowerOfTwo(bs) && isPowerOfTwo(assoc)){
        Data::getInstance()->cache->setCache(cs, bs, assoc);
    }
    else{
        qDebug()<<"reached this point B";
        Data::getInstance()->cache->valid=false;
    }
    close();
}
