#ifndef UTILS_H
#define UTILS_H

#include <QString>

class Utils
{
public:
    Utils();
    static QString formatDuration(qint64 duration);
};

#endif // UTILS_H
