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
    void launchGame();
    void loadFromFile();
private:

    Ui::Launcher *ui;
    QListWidget* listWidget;
    QListWidget* HideListWidget;
    QVBoxLayout* layout;
    QPushButton* addApp;
    QPushButton* bStartGame;


};
#endif // LAUNCHER_H
