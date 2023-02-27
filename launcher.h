#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QMainWindow>
#include <QListWidget>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QPushButton>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QProcess>
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

    //QFileIconProvider* provider;
    //QIcon* icon;
};
#endif // LAUNCHER_H
