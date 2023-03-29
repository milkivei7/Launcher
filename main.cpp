#include "launcher.h"
#include "database.h"
#include <QSqlDatabase>
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dataBase db;
    Launcher w;

    QFile style("C:\\Users\\dark_\\Desktop\\QT Projects\\Launcher\\style.css");
    // градиент в qt: qlineargradient( x1:0 y1:1, x2:1 y2:0, stop:0 #2761A0, stop:1 #9C4588);
    style.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(style.readAll());
    w.setStyleSheet(styleSheet);
    w.show();
    return a.exec();
}
