#include "database.h"

dataBase::dataBase(QObject *parent)
    : QObject{parent}
{

    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=C:/Users/dark_/Desktop/QT Projects/Launcher/SQL Tables/InfoGamesDB.accdb");
    db.open();
    qDebug()<<"Data base is open"<<Qt::endl;
}

dataBase::~dataBase()
{
    db.close();
    qDebug()<<"data base is close"<<Qt::endl;
}


