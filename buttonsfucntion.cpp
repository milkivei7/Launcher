#include "buttonsfucntion.h"

ButtonsFucntion::ButtonsFucntion(QObject *parent)
    : QObject{parent}
{

}

void ButtonsFucntion::addGame()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr,  "Выберите приложение",
                                                   QString(), "Исполняемые файлы (*.exe)");
    QString NameFile = QFileInfo    (fileName).baseName();

    // Проверяем выбран ли файл
    if (fileName.isEmpty())
        return;

    QFileIconProvider provider;
    QIcon icon;

    // Добавляем путь к приложению в список
    item =  new QListWidgetItem     ();
    icon =  provider.icon(QFileInfo (fileName));
    item -> setIcon                 (icon);
    item -> setText                 (NameFile);
    emit    appAdded                (fileName, item);
}

