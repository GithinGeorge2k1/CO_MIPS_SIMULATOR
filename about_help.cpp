#include "about_help.h"
#include "ui_about_help.h"
#include <QFile>

About_Help::About_Help(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About_Help)
{

    ui->setupUi(this);

    QFile html(":/Html/Html/index.html");
    html.open(QFile::ReadOnly);
    QString text=html.readAll();
    ui->textEdit->setHtml(text);

    ui->textEdit->setReadOnly(true);
}

About_Help::~About_Help()
{
    delete ui;
}

void About_Help::on_OK_clicked()
{
    this->close();
}
