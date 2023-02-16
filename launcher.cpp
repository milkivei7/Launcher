#include "launcher.h"
#include "./ui_launcher.h"

Launcher::Launcher(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Launcher)
{
    //infoNameGames.open(QIODevice::WriteOnly | QIODevice::Text);

    //(infoNameGames.isOpen())?qDebug()<<"file is open\n":qDebug()<<"file is'n open\n";
    //loadFromFile();




    ui->setupUi(this);
    addApp = ui->pushButton;
    addApp->setText("AddApp");

    bStartGame = ui->bLaunchGame;
    bStartGame->setText("Launch");

    listWidget = ui->lastWidget;
    HideListWidget = ui->hideWidget;
    HideListWidget->hide();


    connect(addApp, SIGNAL(clicked()), this, SLOT(addGame()));
    connect(bStartGame, SIGNAL(clicked()), this, SLOT(launchGame()));

    loadFromFile();
}

Launcher::~Launcher()
{
    delete ui;
}

void Launcher::addGame()
{
    // Открываем диалог выбора файла
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите приложение",
                                                   QString(), "Исполняемые файлы (*.exe)");
    // Проверяем выбран ли файл
    if (fileName.isEmpty())
        return;

   // Добавляем путь к приложению в список
    listWidget->addItem(fileName.right(fileName.size()-fileName.lastIndexOf("/")-1));
    HideListWidget->addItem(fileName);

    QFile infoNameGames("C:\\Users\\dark_\\Desktop\\QT Projects\\infoNameGames.txt");
    QFile srcNameGames("C:\\Users\\dark_\\Desktop\\QT Projects\\srcNameGames.txt");

    if (infoNameGames.open(QIODevice::Append | QIODevice::Text) && srcNameGames.open(QIODevice::Append | QIODevice::Text)){
        qDebug()<<"Запись в файл началась\n";

        QTextStream writeInFileInfoGames(&infoNameGames);
        QTextStream writeInFileSrcGames(&srcNameGames);

        writeInFileInfoGames<<fileName.right(fileName.size()-fileName.lastIndexOf("/")-1)<<Qt::endl;
        writeInFileSrcGames<<fileName<<Qt::endl;
    }else qDebug()<<"При записи в файл произошла ошибка\n";

    infoNameGames.close();
    srcNameGames.close();


}
void Launcher::launchGame()
{
    int index = listWidget->currentRow();
    if (index == -1) return;
    QString PathFile = HideListWidget->item(index)->text();
    if (PathFile.isEmpty()) return;
    QProcess::startDetached(PathFile);

}

void Launcher::loadFromFile()
{
    QFile infoNameGames("C:\\Users\\dark_\\Desktop\\QT Projects\\infoNameGames.txt");
    QFile srcNameGames("C:\\Users\\dark_\\Desktop\\QT Projects\\srcNameGames.txt");

    if (infoNameGames.open(QIODevice::ReadOnly | QIODevice::Text) &&
            srcNameGames.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"Запись из файла началась\n";

        QTextStream streamFromFileInfoGames(&infoNameGames);
        QTextStream streamFromFileSrcGames(&srcNameGames);
        while(!(infoNameGames.atEnd() && srcNameGames.atEnd() ))
        {
            QString readLineNameGames = infoNameGames.readLine();
            readLineNameGames.resize(readLineNameGames.size()-1);

            QString readLineSrcGames = srcNameGames.readLine();
            readLineSrcGames.resize(readLineSrcGames.size()-1);

            listWidget->addItem(readLineNameGames);
            HideListWidget->addItem(readLineSrcGames);
        }
    }else qDebug()<<"При записи из файла произошла ошибка\n";

    infoNameGames.close();
    srcNameGames.close();
}

