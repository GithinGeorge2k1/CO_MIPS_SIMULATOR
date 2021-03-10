#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionReinitialize_and_Load_File_triggered();
    void on_actionQuit_triggered();
    void on_actionInitialize_triggered();

    void on_actionSee_LabelMap_triggered();

    void on_actionSee_DataMap_triggered();

    void on_actionClear_Registers_triggered();

    void on_actionRun_triggered();

private:
    Ui::MainWindow *ui;
    static bool ValidCodePresent;
//methods
private:
    void refreshAllPanels();
    void initialize();
};
#endif // MAINWINDOW_H
