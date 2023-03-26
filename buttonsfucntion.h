#ifndef BUTTONSFUCNTION_H
#define BUTTONSFUCNTION_H

#include <QObject>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QFileIconProvider>



class ButtonsFucntion : public QObject
{
    Q_OBJECT
public:
    explicit ButtonsFucntion(QObject *parent = nullptr);
public slots:
    void addGame();
private:
    QListWidgetItem* item;
signals:
    void appAdded(const QString &fileName, QListWidgetItem *item);

};

#endif // BUTTONSFUCNTION_H
