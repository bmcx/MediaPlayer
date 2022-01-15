#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>

class PlayerController
{
public:
    PlayerController(QObject *parent);

    QMediaPlayer *instance;

    void togglePlayPause();
    void stop();
    void addToPlaylist(QStringList paths);

private:
    QMediaPlaylist *playlist;

};

#endif // PLAYERCONTROLLER_H
