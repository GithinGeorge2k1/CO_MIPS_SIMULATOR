#include "about_help.h"
#include "ui_about_help.h"

About_Help::About_Help(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About_Help)
{
    ui->setupUi(this);
}

About_Help::~About_Help()
{
    delete ui;
}

void About_Help::on_OK_clicked()
{
    this->close();
}
