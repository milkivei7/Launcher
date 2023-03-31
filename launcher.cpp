#include "launcher.h"
#include "./ui_launcher.h"

Launcher::Launcher(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Launcher)
{

    ui -> setupUi(this);


    addApp        =  ui->pushButton;
    bStartGame    =  ui->bLaunchGame;
    bDeleteGame   =  ui->deleteGame;

    bStartGame    -> setText ("\u25B6");
    addApp        -> setText ("AddApp");
    bDeleteGame   -> setText ("Delete");
    ui -> label   -> setText ("Time: ");

    listWidget      =  ui -> lastWidget;
    HideListWidget  =  ui -> hideWidget;
    HideListWidget  -> hide();


    vAllLayout = ui -> vAllLayout;
    vLayout    = ui -> verticalLayout;
    hLayout    = ui -> horizontalLayout;

    ui -> horizontalLayout_2 -> addWidget(bStartGame);
    ui -> horizontalLayout_2 -> addWidget(ui->label);
    ui -> horizontalLayout_2 -> addWidget(ui->timeLabel);

    hLayout -> addWidget(addApp);
    hLayout -> addWidget(bDeleteGame);

    vLayout->    addWidget(listWidget);
    vAllLayout-> addLayout(vLayout);
    vAllLayout-> addLayout(hLayout);

    //timer
    time         = 0;
    timer        = new QTimer   (this);
    startProcess = new QProcess (this);

    connect(timer,        SIGNAL(timeout()),                          this,     SLOT(TimerSlot()));
    connect(addApp,       &QPushButton::clicked,                      buttons,  &ButtonsFucntion::addGame);
    connect(buttons,      &ButtonsFucntion::appAdded,                 this,     &Launcher::addGame);

    connect(listWidget,   SIGNAL(itemClicked(QListWidgetItem*)),      this,     SLOT(onItemClicked()));
    connect(bStartGame,   SIGNAL(clicked()),                          this,     SLOT(launchGame()));
    connect(bDeleteGame,  SIGNAL(clicked()),                          this,     SLOT(deleteGame()));
    connect(startProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this,     SLOT(isExitProcess()));

    loadFromFile();
}

Launcher::~Launcher()
{
    delete ui;
}

void Launcher::addGame(const QString& fileName, QListWidgetItem* item)
{
    // Открываем диалог выбора файла
    QString NameFile =  QFileInfo (fileName).baseName();
    listWidget       -> addItem   (item);
    HideListWidget   -> addItem   (fileName);

    //Подключаем Базу данных
    QSqlQuery query;
    query.prepare   ("INSERT INTO InfoGames (InfoGame, SrcGame) "
                           "VALUES (:InfoGame, :SrcGame)");

    query.bindValue (":InfoGame", NameFile);
    query.bindValue (":SrcGame" , fileName);
    query.exec();

}
void Launcher::launchGame()
{
    //startProcess = new QProcess;
    int index = listWidget -> currentRow();
    if (index == -1)
        return;

    PathFile         =  new QString;
    *PathFile        =  HideListWidget -> item (index) ->text();
    nameStartProcess =  PathFile;

    if (PathFile -> isEmpty () ) return;

    startProcess -> start (*PathFile);

    if (startProcess -> waitForStarted () )
    {
        timer    -> start(1000);
        int pid  =  startProcess -> processId();
        qDebug() << "Приложение запустилось!\n" << startProcess -> state() << " Pid: " << pid << Qt::endl;
    }

}

void Launcher::bAnimation(QPushButton &Button)
{
    QPropertyAnimation *animation = new QPropertyAnimation(&Button, "size");

    animation -> setDuration    (500);
    animation -> setStartValue  (QColor(255, 0, 0));
    animation -> setEndValue    (QColor(0, 0, 0));
    animation -> setEasingCurve (QEasingCurve::InOutCubic);

}

void Launcher::deleteGame()
{

    qDebug() << "Data for delete is open" << Qt::endl;

    QSqlQuery query;
    int       index = listWidget -> currentRow();
    qDebug()  << "Number index = " << index<<Qt::endl;

    //передаем по индексу элемент
    item                          = listWidget     -> item (index);
    QListWidgetItem* itemSrcGames = HideListWidget -> item (index);

    //Название удаляемого элемента
    QString    NameToDelete = item -> text();
    qDebug()<< NameToDelete << Qt::endl;

    query.prepare   ("DELETE FROM InfoGames WHERE InfoGame=:InfoGame");
    query.bindValue (":InfoGame", NameToDelete);
    query.exec();

    delete item;
    delete itemSrcGames;
}

void Launcher::loadFromFile()
{
    QSqlQuery  query;
    query.exec ("SELECT InfoGame, SrcGame FROM InfoGames");
    if ( query.isActive() )
    {
        qDebug() << "query for load is active" << Qt::endl;
        while( query.next() )
        {
            QString readLineNameGames =  query.value(0).toString();
            QString readLineSrcGames  =  query.value(1).toString();

            item = new QListWidgetItem;
            QFileIconProvider readProvider;
            QIcon readIcon;

            readIcon       =  readProvider.icon (QFileInfo(readLineSrcGames));
            item           -> setIcon           (readIcon);
            item           -> setText           (readLineNameGames);
            listWidget     -> addItem           (item);
            HideListWidget -> addItem           (readLineSrcGames);
        }

    }else
        qDebug() << "error: " <<query.lastError();
}

void Launcher::isStartProcess()
{
    qDebug() << " Application is open " << Qt::endl;
}

void Launcher::isExitProcess()
{
    if ( startProcess -> atEnd() )
    {               
        qDebug() << "Process finished successfully" <<Qt::endl;
        qDebug() << "name start = "    <<*PathFile  <<Qt::endl;

        timer->stop();
        QSqlQuery query;

        query.prepare   ("UPDATE InfoGames SET [Time]=:Time WHERE [SrcGame]=:SrcGame");
        query.bindValue (":Time", time);
        query.bindValue (":SrcGame",*PathFile);
        query.exec();
    }

    else{
        qDebug( "Process finished with error  -  ");
        qDebug()<<startProcess->state()<<Qt::endl;
    }
}
void Launcher::TimerSlot()
{
    time++;
    timeText =  ui -> timeLabel;
    timeText -> setText (QString::number(time));
}

void Launcher::onItemClicked()
{

    int index = listWidget -> currentRow ();
    item      = listWidget -> item       (index);

    QSqlQuery query;
    QString   Name  = item->text();

    query.prepare   ("SELECT Time FROM InfoGames WHERE [InfoGame]=:InfoGame");
    query.bindValue (":InfoGame", Name);

    if ( !query.exec() )
        qDebug() << "Error:" << query.lastError().text();

    while (query.next())
        time = query.value(0).toInt();

    ui -> timeLabel -> setText (QString::number (time) );

}
