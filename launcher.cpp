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
    bStartGame->setText("\u25B6");

    bDeleteGame = ui->deleteGame;
    bDeleteGame->setText("Delete");

    ui->label->setText("Time: ");

    listWidget = ui->lastWidget;

    HideListWidget = ui->hideWidget;
    HideListWidget->hide();


    vAllLayout = ui->vAllLayout;
    vLayout= ui->verticalLayout;
    hLayout = ui->horizontalLayout;

    ui->horizontalLayout_2->addWidget(bStartGame);
    ui->horizontalLayout_2->addWidget(ui->label);
    ui->horizontalLayout_2->addWidget(ui->timeLabel);
    //hLayout->addWidget(bStartGame);
    hLayout->addWidget(addApp);
    hLayout->addWidget(bDeleteGame);


    vLayout->addWidget(listWidget);
    vAllLayout->addLayout(vLayout);
    vAllLayout->addLayout(hLayout);

    //timer
    time = 0;
    timer= new QTimer(this);
    startProcess = new QProcess(this);

    connect(timer, SIGNAL(timeout()),this,SLOT(TimerSlot()));
    connect(addApp, SIGNAL(clicked()), this, SLOT(addGame()));

    connect(listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(onItemClicked()));

    connect(bStartGame, SIGNAL(clicked()), this, SLOT(launchGame()));
    connect(bDeleteGame, SIGNAL(clicked()), this, SLOT(deleteGame()));

    //connect(startProcess,SIGNAL(started()),this,SLOT(isStartProcess()));

    connect(startProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(isExitProcess()));

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

    item = new QListWidgetItem();
    QFileIconProvider provider;
    //Добавляем к нему иконку приложения
    QIcon icon;
    icon = provider.icon(QFileInfo(fileName));
    item->setIcon(icon);

    item->setText(NameFile);
    listWidget->addItem(item);

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
        //query.bindValue(":Time", i);
        query.exec();
        db.close();
    }
    else qDebug()<<"Data base isn't open: "<<db.lastError();

}
void Launcher::launchGame()
{
    //startProcess = new QProcess;
    int index = listWidget->currentRow();
    if (index == -1) return;

    PathFile=new QString;
    *PathFile = HideListWidget->item(index)->text();

    nameStartProcess = PathFile;
    //qDebug()<<*nameStartProcess<<Qt::endl;
    if (PathFile->isEmpty()) return;

    startProcess->start(*PathFile);
    if (startProcess->waitForStarted()){

        timer->start(1000);
        int pid = startProcess->processId();
        qDebug()<<"Приложение запустилось!\n"<<startProcess->state()<<" Pid: "<<pid<<Qt::endl;
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
        item = listWidget->item(index);
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



                item = new QListWidgetItem;
                QFileIconProvider readProvider;
                QIcon readIcon;

                readIcon = readProvider.icon(QFileInfo(readLineSrcGames));
                item->setIcon(readIcon);
                item->setText(readLineNameGames);

                listWidget->addItem(item);
                HideListWidget->addItem(readLineSrcGames);
            }

        }else qDebug()<<"error: "<<query.lastError();
        db.close();
    }
    else qDebug()<<"Data base isn't open: "<<db.lastError();
}

void Launcher::isStartProcess()
{
    qDebug()<<" Application is open "<<Qt::endl;

}

void Launcher::isExitProcess()
{

    //if (startProcess->exitStatus() == QProcess::NormalExit && startProcess->exitCode() == 0)
    if (startProcess->atEnd())
    {
               qDebug("Process finished successfully");
               qDebug()<<"name start = "<<*PathFile<<Qt::endl;
               timer->stop();

               if (db.open())
               {
                   qDebug()<<"Data base is open"<<Qt::endl;
                   QSqlQuery query;
                   //query.prepare("INSERT INTO InfoGames (Time) VALUES (:Time) WHERE SrcGame = :SrcGame");
                   query.prepare("UPDATE InfoGames SET [Time]=:Time WHERE [SrcGame]=:SrcGame");

                   query.bindValue(":Time", time);
                   query.bindValue(":SrcGame",*PathFile);
                   //query.bindValue(":Time", i);
                   query.exec();

               }
                else qDebug()<<"Errrrrrrror SQL "<<db.lastError();
               db.close();

    }
           else{

               qDebug("Process finished with error  -  ");
               qDebug()<<startProcess->state()<<Qt::endl;
                }

    //qDebug()<<" Application is exit "<<Qt::endl;

}
void Launcher::TimerSlot()
{
    time++;
    timeText = ui->timeLabel;
    timeText->setText(QString::number(time));


}

void Launcher::onItemClicked()
{
    int index = listWidget->currentRow();
    item = listWidget->item(index);

    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=C:/Users/dark_/Desktop/QT Projects/Launcher/SQL Tables/InfoGamesDB.accdb");

    if (db.open())
    {
        qDebug()<<"Data for timer is open"<<Qt::endl;
        QSqlQuery query;
        QString Name = item->text();
        query.prepare("SELECT Time FROM InfoGames WHERE [InfoGame]=:InfoGame");
        query.bindValue(":InfoGame", Name);
        if (!query.exec())
        {
            qDebug() << "Error:" << query.lastError().text();
        }
        while (query.next())
        {
            time = query.value(0).toInt();
        }

        //ui->timeLabel->setText(QString::number(9999));
    }else qDebug()<<"DB not open "<<db.lastError()<<Qt::endl;
    db.close();
    ui->timeLabel->setText(QString::number(time));

}
