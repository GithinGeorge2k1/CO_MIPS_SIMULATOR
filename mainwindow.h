#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "about_help.h"
#include <QListWidget>
#include <QCheckBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    bool isTimeLineLocked;
    int tableIndex;
    static MainWindow* getInstance();
    void setNewTable();
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionReinitialize_and_Load_File_triggered();
    void on_actionQuit_triggered();
    void on_actionClear_Registers_triggered();
    void on_actionRun_triggered();
    void on_actionRun_Step_By_Step_triggered();
    void on_actionHelp_triggered();
    void on_actionEnable_Forwarding_triggered();
    void ForwardingEnabled();

private:

    Ui::MainWindow *ui;
    static bool ValidCodePresent;
    QCheckBox* fowd;
    bool isExecuting;
    About_Help* helpwindow;
    QTableWidget** timeline;
    QListWidget* stallList;
    static MainWindow* obj;

private:
    void refreshAllPanels();
    void initialize();
};
#endif // MAINWINDOW_H
