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
    int isBytes=ui->comboBox->currentIndex(); //0-KB 1-Bytes

    Data* D=Data::getInstance();
    bool one=false,two=false,three=false,four=false,five=false;
    int cs=ui->lineEdit->text().toInt(&one);
    int bs=ui->lineEdit_2->text().toInt(&two);
    int assoc=ui->lineEdit_3->text().toInt(&three);
    int sl=ui->lineEdit_4->text().toInt(&four);
    int ll=ui->lineEdit_5->text().toInt(&five);
    if(one && two && three && isPowerOfTwo(bs)>=2 && isPowerOfTwo(assoc)>=0 && sl>=0 && ll>=0)
    {
        if((isPowerOfTwo(cs)>=2 && isBytes==1)||(isPowerOfTwo(cs)>=0 && isBytes==0)){
            if(isBytes==0)
                cs*=1024;
            D->cache[0]->setCache(cs, bs, assoc, ll, sl);
        }
        else{
            D->cache[0]->valid=false;
            close();
            return;
        }
    }
    else
    {
        D->cache[0]->valid=false;
        close();
        return;
    }
    one=false;two=true;three=false;four=false;five=false;
    cs=ui->lineEdit_6->text().toInt(&one);
    assoc=ui->lineEdit_7->text().toInt(&three);
    sl=ui->lineEdit_8->text().toInt(&four);
    ll=ui->lineEdit_9->text().toInt(&five);
    if(one && two && three && isPowerOfTwo(bs)>=2 && isPowerOfTwo(assoc)>=0 && sl>=0 && ll>=0)
    {
        if((isPowerOfTwo(cs)>=2 && isBytes==1)||(isPowerOfTwo(cs)>=0 && isBytes==0)){
            if(isBytes==0)
                cs*=1024;
            D->cache[1]->setCache(cs, bs, assoc, ll, sl);
        }
        else{
            D->cache[1]->valid=false;
            close();
            return;
        }
    }
    else
    {
        D->cache[1]->valid=false;
        close();
        return;
    }
    close();
}
