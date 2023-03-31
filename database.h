#ifndef DATABASE_H
#define DATABASE_H

#include        <QObject>
#include   <QSqlDatabase>
#include         <QDebug>
#include      <QSqlError>

class dataBase : public QObject
{
    Q_OBJECT
public:
    explicit dataBase(QObject *parent = nullptr);
    ~dataBase();
    QSqlDatabase db;
private:

    void openDataBase();
    void exitDataBase();

signals:

};

#endif // DATABASE_H
