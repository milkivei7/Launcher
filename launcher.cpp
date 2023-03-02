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

    //bStartGame->stackUnder(listWidget);

    //bAnimation(*bStartGame);

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
    QString NameFile = QFileInfo(fileName).fileName();



    // Проверяем выбран ли файл
    if (fileName.isEmpty())
        return;

   // Добавляем путь к приложению в список
    itemListWidget = new QListWidgetItem();

    QFileIconProvider provider;
    QIcon icon;
    icon = provider.icon(QFileInfo(fileName));
    itemListWidget->setIcon(icon);

    //itemListWidget->setText(fileName.right(fileName.size()-fileName.lastIndexOf("/")-1));
    itemListWidget->setText(NameFile);
    listWidget->addItem(itemListWidget);

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
    //open files with name and src
    QFile infoNameGames("C:\\Users\\dark_\\Desktop\\QT Projects\\infoNameGames.txt");
    QFile srcNameGames("C:\\Users\\dark_\\Desktop\\QT Projects\\srcNameGames.txt");
    // Create buffer fiels
    QFile bufferInfoGames("bufferInfoGames.txt");
    QFile bufferSrcGames("buffersrcGames.txt");

    //Open files
    if (infoNameGames.open(QIODevice::ReadWrite | QIODevice::Text  ) &&
            srcNameGames.open(QIODevice::ReadWrite | QIODevice::Text )&&
            bufferInfoGames.open(QIODevice::ReadWrite | QIODevice::Text)&&
            bufferSrcGames.open(QIODevice::ReadWrite | QIODevice::Text))
    {

        qDebug()<<"Файлы открылись успешно"<<Qt::endl;
        int index = listWidget->currentRow();

        QString ReadAllInfo = infoNameGames.readAll();
        QString ReadAllSrc = srcNameGames.readAll();

        //create TextStream with info and src
        QTextStream streamBufferInfo(&bufferInfoGames);
        QTextStream streamBufferSrc(&bufferSrcGames);

        //передаем по индексу элемент
        QListWidgetItem* item = listWidget->item(index);
        QListWidgetItem* itemSrcGames = HideListWidget->item(index);
        //Название удаляемого элемента
        QString NameToDelete = item->text();
        qDebug()<<NameToDelete<<Qt::endl;

        //Переходим в начало файлов
        infoNameGames.seek(0);
        srcNameGames.seek(0);

        while(!(infoNameGames.atEnd()&&srcNameGames.atEnd())){



            //Присвоение строки к переменной ReadLineInfo
            QString ReadLineInfo = infoNameGames.readLine();
            ReadLineInfo.resize(ReadLineInfo.size()-1);

            //Присвоение строки к переменной ReadLineSrc
            QString ReadLineSrc = srcNameGames.readLine();
            ReadLineSrc.resize(ReadLineSrc.size()-1);


            //ReadLineSrc.removeLast();
            if (NameToDelete == ReadLineInfo)
            {

                qDebug()<<"Удаляем строчку"<<ReadLineInfo<<"  "<<ReadLineSrc<<Qt::endl;
                ReadLineInfo.resize(ReadLineInfo.size());
                ReadAllInfo.remove(ReadLineInfo);

                ReadLineSrc.resize(ReadLineSrc.size());
                ReadAllSrc.remove(ReadLineSrc);

            }
            else
            {
                qDebug()<<"Другая строчка"<<Qt::endl;
                streamBufferInfo<<ReadLineInfo<<Qt::endl;
                streamBufferSrc<<ReadLineSrc<<Qt::endl;
            }

        }
        infoNameGames.seek(0);
        infoNameGames.write(ReadAllInfo.toUtf8());
        srcNameGames.seek(0);
        srcNameGames.write(ReadAllSrc.toUtf8());

        //Удаляем элемент item из QListWidget'ов
        delete item;
        delete itemSrcGames;
    }
    else
    {
        qDebug()<<"При открытии файлов произошла ошибка"<<Qt::endl;
        return;
    }


    //Очищаем файл

    infoNameGames.close();
    srcNameGames.close();
    bufferInfoGames.close();
    bufferSrcGames.close();
    //infoNameGames.resize(0); srcNameGames.resize(0);infoNameGames.seek(0);srcNameGames.seek(0);bufferInfoGames.seek(0);bufferSrcGames.seek(0);

    if(infoNameGames.open(QIODevice::WriteOnly|QIODevice::Truncate)&&srcNameGames.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        infoNameGames.close();
        srcNameGames.close();
    }

    if(infoNameGames.open(QIODevice::Append|QIODevice::Text)&&srcNameGames.open(QIODevice::Append)|QIODevice::Text
            && bufferInfoGames.open(QIODevice::ReadWrite|QIODevice::Text)&&bufferSrcGames.open(QIODevice::ReadWrite|QIODevice::Text))
    {

        qDebug()<<"Файл с буффером успешно открыт"<<Qt::endl;
        QTextStream outInfoNameGames(&infoNameGames);
        QTextStream outSrcGames(&srcNameGames);

        while(!(bufferInfoGames.atEnd()&&bufferSrcGames.atEnd()))
        {
            QString lineInfoGames = bufferInfoGames.readLine();
            lineInfoGames.resize(lineInfoGames.size()-1);
            outInfoNameGames<<lineInfoGames<<Qt::endl;



            QString lineSrcGames = bufferSrcGames.readLine();
            lineSrcGames.resize(lineSrcGames.size()-1);
            outSrcGames<<lineSrcGames<<Qt::endl;
        }
    }
    infoNameGames.close();
    srcNameGames.close();
    bufferInfoGames.close();
    bufferSrcGames.close();

    if(bufferInfoGames.open(QIODevice::WriteOnly|QIODevice::Truncate)&&bufferSrcGames.open(QIODevice::WriteOnly|QIODevice::Truncate))
        qDebug()<<"Файлы буфера очищены"<<Qt::endl;
    bufferInfoGames.close();
    bufferSrcGames.close();
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

            QListWidgetItem* readListWidgetItem = new QListWidgetItem;
            QFileIconProvider readProvider;
            QIcon readIcon;

            readIcon = readProvider.icon(QFileInfo(readLineSrcGames));
            readListWidgetItem->setIcon(readIcon);
            readListWidgetItem->setText(readLineNameGames);


            listWidget->addItem(readListWidgetItem);
            HideListWidget->addItem(readLineSrcGames);
        }
    }else qDebug()<<"При записи из файла произошла ошибка\n";

    infoNameGames.close();
    srcNameGames.close();
}
