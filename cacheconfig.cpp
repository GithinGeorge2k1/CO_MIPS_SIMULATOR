#include "cacheconfig.h"
#include "ui_cacheconfig.h"
#include "Data.h"


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
    int cs=ui->lineEdit->text().toInt();
    int bs=ui->lineEdit_2->text().toInt();
    int assoc=ui->lineEdit_3->text().toInt();
    Data::getInstance()->setCache(cs, bs, assoc);
    close();
}
