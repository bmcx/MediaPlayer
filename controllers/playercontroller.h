#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QMessageBox>

class PlayerController
{
public:
    PlayerController(QObject *parent);

    QMediaPlayer *instance;
    QStandardItemModel *playlistModel;

    void togglePlayPause();
    void stop();
    void addToPlaylist(QStringList paths);
    void laodPlaylist(QString path);
    void clearPlaylist();

public slots:
    void changePositionBy(int value);
    void on_videoError();

private:
    QMediaPlaylist *playlist;

};

#endif // PLAYERCONTROLLER_H
