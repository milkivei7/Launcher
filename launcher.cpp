#include "launcher.h"
#include "./ui_launcher.h"

Launcher::Launcher(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Launcher)
{

    ui->setupUi(this);


    addApp = ui->pushButton;
    addApp->setText("AddApp");

    bStartGame = ui->bLaunchGame;
    bStartGame->setText("Launch");

    bDeleteGame = ui->deleteGame;
    bDeleteGame->setText("Delete");

    listWidget = ui->lastWidget;

    HideListWidget = ui->hideWidget;
    HideListWidget->hide();

    vAllLayout = ui->vAllLayout;
    vLayout= ui->verticalLayout;
    hLayout = ui->horizontalLayout;

    hLayout->addWidget(bStartGame);
    hLayout->addWidget(addApp);
    hLayout->addWidget(bDeleteGame);


    vLayout->addWidget(listWidget);
    vAllLayout->addLayout(vLayout);
    vAllLayout->addLayout(hLayout);

    //timer
    time = 0;
    timer= new QTimer(this);
    connect(timer, SIGNAL(timeout()),this,SLOT(TimerSlot()));
    connect(addApp, SIGNAL(clicked()), this, SLOT(addGame()));
    connect(bStartGame, SIGNAL(clicked()), this, SLOT(launchGame()));
    connect(bDeleteGame, SIGNAL(clicked()), this, SLOT(deleteGame()));

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

    QString NameFile = QFileInfo(fileName).baseName();
    // Проверяем выбран ли файл
    if (fileName.isEmpty())
        return;
   // Добавляем путь к приложению в список

    itemListWidget = new QListWidgetItem();
    QFileIconProvider provider;
    //Добавляем к нему иконку приложения
    QIcon icon;
    icon = provider.icon(QFileInfo(fileName));
    itemListWidget->setIcon(icon);

    itemListWidget->setText(NameFile);
    listWidget->addItem(itemListWidget);

    HideListWidget->addItem(fileName);

    //Подключаем Базу данных
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=C:/Users/dark_/Desktop/QT Projects/Launcher/SQL Tables/InfoGamesDB.accdb");

    if (db.open())
    {
        qDebug()<<"Data base is open"<<Qt::endl;
        QSqlQuery query;
        query.prepare("INSERT INTO InfoGames (InfoGame, SrcGame) "
                           "VALUES (:InfoGame, :SrcGame)");
        query.bindValue(":InfoGame", NameFile);
        query.bindValue(":SrcGame", fileName);
        query.exec();
        db.close();
    }
    else qDebug()<<"Data base isn't open: "<<db.lastError();

}
void Launcher::launchGame()
{
    startProcess = new QProcess(this);
    int index = listWidget->currentRow();
    if (index == -1) return;


    QString PathFile = HideListWidget->item(index)->text();
    if (PathFile.isEmpty()) return;

    startProcess->start(PathFile);
    if (startProcess->waitForStarted()){

        timer->start(1000);
        int pid = startProcess->processId();
        qDebug()<<"Приложение запустилось!\n"<<startProcess->state()<<" Pid: "<<pid<<Qt::endl;

    }
    if (startProcess->waitForFinished()) {
        qDebug()<<"Приложение закрылось!\n";
        timer->stop();
    }
}



void Launcher::bAnimation(QPushButton &Button)
{
    QPropertyAnimation *animation = new QPropertyAnimation(&Button, "size");

    animation->setDuration(500);
    animation->setStartValue(QColor(255, 0, 0));
    animation->setEndValue(QColor(0, 0, 0));
    animation->setEasingCurve(QEasingCurve::InOutCubic);
}

void Launcher::deleteGame()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=C:/Users/dark_/Desktop/QT Projects/Launcher/SQL Tables/InfoGamesDB.accdb");

    if (db.open())
    {
        qDebug()<<"Data for delete is open"<<Qt::endl;
        QSqlQuery query;
        int index = listWidget->currentRow();
        qDebug()<<"number index = "<<index<<Qt::endl;
        //передаем по индексу элемент
        QListWidgetItem* item = listWidget->item(index);
        QListWidgetItem* itemSrcGames = HideListWidget->item(index);
        //Название удаляемого элемента
        QString NameToDelete = item->text();
        qDebug()<<NameToDelete<<Qt::endl;

        query.prepare("DELETE FROM InfoGames WHERE InfoGame=:InfoGame");
        query.bindValue(":InfoGame", NameToDelete);
        query.exec();

        delete item;
        delete itemSrcGames;
        db.close();

    }
    else qDebug()<<"Data base isn't open: "<<db.lastError();
}

void Launcher::loadFromFile()
{

    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=C:/Users/dark_/Desktop/QT Projects/Launcher/SQL Tables/InfoGamesDB.accdb");
    if (db.open())
    {
        qDebug()<<"Data for load is open"<<Qt::endl;
        QSqlQuery query;
        query.exec("SELECT InfoGame, SrcGame FROM InfoGames");
        if (query.isActive())
        {
            qDebug()<<"query for load is active"<<Qt::endl;

            while(query.next())
            {
                QString readLineNameGames = query.value(0).toString();

                QString readLineSrcGames = query.value(1).toString();

                QListWidgetItem* readListWidgetItem = new QListWidgetItem;
                QFileIconProvider readProvider;
                QIcon readIcon;

                readIcon = readProvider.icon(QFileInfo(readLineSrcGames));
                readListWidgetItem->setIcon(readIcon);
                readListWidgetItem->setText(readLineNameGames);

                listWidget->addItem(readListWidgetItem);
                HideListWidget->addItem(readLineSrcGames);
            }

        }else qDebug()<<"error: "<<query.lastError();
        db.close();
    }
    else qDebug()<<"Data base isn't open: "<<db.lastError();
}

bool Launcher::isStart()
{
    if(startProcess->state()!=QProcess::NotRunning)
        return true;
    else return false;
}
void Launcher::TimerSlot()
{
    time++;
    ui->timeLabel->setText(QString::number(time));

}
