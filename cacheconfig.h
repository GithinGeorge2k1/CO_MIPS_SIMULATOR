#ifndef CACHECONFIG_H
#define CACHECONFIG_H

#include <QDialog>

namespace Ui {
class CacheConfig;
}

class CacheConfig : public QDialog
{
    Q_OBJECT

public:
    explicit CacheConfig(QWidget *parent = nullptr);
    ~CacheConfig();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CacheConfig *ui;
};

#endif // CACHECONFIG_H
