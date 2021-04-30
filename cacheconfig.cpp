#include "cacheconfig.h"
#include "ui_cacheconfig.h"
#include "Data.h"
#include <QDebug>
int isPowerOfTwo(int x)// checks if x=2^a (a>0)
{
    if(x<=0)
        return -1;

    int c=0;
    while(x!=1)
    {
        if(x%2!=0)
            return -1;
        c++;
        x=x/2;
    }
    return c;
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
    if(one && two && three && isPowerOfTwo(cs)>=0 && isPowerOfTwo(bs)>=2 && isPowerOfTwo(assoc)>=0)
        Data::getInstance()->cache->setCache(cs, bs, assoc);
    else
        Data::getInstance()->cache->valid=false;
    close();
}
