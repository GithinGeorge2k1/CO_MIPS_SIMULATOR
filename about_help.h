#ifndef ABOUT_HELP_H
#define ABOUT_HELP_H

#include <QDialog>

namespace Ui {
class About_Help;
}

class About_Help : public QDialog
{
    Q_OBJECT

public:
    explicit About_Help(QWidget *parent = nullptr);
    ~About_Help();

private slots:
    void on_OK_clicked();

private:
    Ui::About_Help *ui;
};

#endif // ABOUT_HELP_H
