#include "utils.h"

Utils::Utils()
{

}

QString Utils::formatDuration(qint64 duration)
{
    int totalSeconds = duration/1000;
    int seconds = totalSeconds % 60;
    int minutes = (totalSeconds / 60) % 60;
    int hours = (totalSeconds / 60 / 60);

    QString timeString = QString("%1:%2:%3")
                             .arg(hours, 2, 10, QChar('0'))
                             .arg(minutes, 2, 10, QChar('0'))
                             .arg(seconds, 2, 10, QChar('0'));
    return timeString;
}
