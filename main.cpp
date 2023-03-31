#include    "launcher.h"
#include    "database.h"
#include  <QSqlDatabase>
#include  <QApplication>



int main(int argc, char *argv[])
{
    QApplication a  (argc, argv);
    dataBase db;
    Launcher w;

    QFile style     ("C:\\Users\\dark_\\Desktop\\QT Projects\\Launcher\\style.css");
    style.open      (QFile::ReadOnly);
    QString styleSheet = QLatin1String (style.readAll());
    w.setStyleSheet (styleSheet);
    w.show ();
    return a.exec();
}
