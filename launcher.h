#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QMainWindow>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>

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
    void isExitProcess();
    void isStartProcess();

    //timer
    void TimerSlot();
    //testText
    void onItemClicked();


    void loadFromFile();
    void bAnimation(QPushButton& Button);

private:
    Ui::Launcher *ui;

    QVBoxLayout* vLayout;
    QVBoxLayout* vAllLayout;
    QHBoxLayout* hLayout;

    QLabel* timeText;

    QListWidget* listWidget;
    QListWidget* HideListWidget;
    //QListWidgetItem* itemListWidget;
    QListWidgetItem* item;

    QVBoxLayout* layout;
    QPushButton* bDeleteGame;
    QPushButton* addApp;
    QPushButton* bStartGame;

    QString* nameStartProcess;
    //process
    QProcess* startProcess;
    QString* PathFile;
    //timer
    QTimer* timer;
    int time;

    QSqlDatabase db;

    //QFileIconProvider* provider;
    //QIcon* icon;
};
#endif // LAUNCHER_H
