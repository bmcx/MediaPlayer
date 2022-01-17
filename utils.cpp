#include "utils.h"

Utils::Utils() {}

/// Formats a given duration in milliseconds to hh:mm:ss or mm:ss
/// only if h>0 the hh:mm:ss format will be used
QString Utils::formatDuration(qint64 duration)
{
    QString timeString = "00:00";

    int totalSeconds = duration/1000;
    int seconds = totalSeconds % 60;
    int minutes = (totalSeconds / 60) % 60;
    int hours = (totalSeconds / 60 / 60);

    if(hours > 0){
        timeString = QString("%1:%2:%3")
                         .arg(hours, 2, 10, QChar('0'))
                         .arg(minutes, 2, 10, QChar('0'))
                         .arg(seconds, 2, 10, QChar('0'));
    }else{
        timeString = QString("%1:%2")
                         .arg(minutes, 2, 10, QChar('0'))
                         .arg(seconds, 2, 10, QChar('0'));
    }

    return timeString;
}
