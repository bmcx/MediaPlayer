#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QStandardItemModel>

class PlayerController
{
public:
    PlayerController(QObject *parent);

    QMediaPlayer *instance;
    QStandardItemModel *playlistModel;

    void togglePlayPause();
    void stop();
    void addToPlaylist(QStringList paths);

public slots:
    void changePositionBy(int value);

private:
    QMediaPlaylist *playlist;

};

#endif // PLAYERCONTROLLER_H
