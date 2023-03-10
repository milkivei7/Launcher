#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QMainWindow>
#include <QListWidget>
#include <QVBoxLayout>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QListWidgetItem>
#include <QPushButton>

#include <QFileDialog>
#include <QFile>
#include <QProcess>
#include <QSystemTrayIcon>

#include <QTextStream>


//timer
#include<QTimer>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QPropertyAnimation>
#include <QCoreApplication>
#include <QFileIconProvider>





QT_BEGIN_NAMESPACE
namespace Ui { class Launcher; }
QT_END_NAMESPACE

class Launcher : public QMainWindow
{
    Q_OBJECT

public:
    Launcher(QWidget *parent = nullptr);
    ~Launcher();

private slots:
    void addGame();
    void deleteGame();
    void launchGame();
    bool isStart();

    //timer
    void TimerSlot();

    void loadFromFile();
    void bAnimation(QPushButton& Button);
private:
    Ui::Launcher *ui;

    QVBoxLayout* vLayout;
    QVBoxLayout* vAllLayout;
    QHBoxLayout* hLayout;

    QListWidget* listWidget;
    QListWidget* HideListWidget;
    QListWidgetItem* itemListWidget;

    QVBoxLayout* layout;
    QPushButton* bDeleteGame;
    QPushButton* addApp;
    QPushButton* bStartGame;

    //process
    QProcess* startProcess;

    //timer
    QTimer* timer;
    int time;

    QSqlDatabase db;

    //QFileIconProvider* provider;
    //QIcon* icon;
};
#endif // LAUNCHER_H
