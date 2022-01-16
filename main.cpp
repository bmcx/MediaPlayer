#include "mediaplayer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /// set a custom theme from stylesheets, do not have all the necessary styles
    /// if uncommented below lines, it should work, but some of the UI elements
    /// like, button toggles aren't visible. Also menu icons needs to be changes to white

    // QFile styleFile(":/qss/darkeum.qss");
    // styleFile.open(QFile::ReadOnly);
    // QString style = QLatin1String(styleFile.readAll());
    // a.setStyleSheet(style);

    a.setStyle("fusion");

    MediaPlayer w;
    w.ensurePolished();
    w.show();
    return a.exec();
}
