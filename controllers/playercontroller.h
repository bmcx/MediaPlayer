#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QMediaPlayer>
#include <QFileDialog>

class PlayerController
{
public:
    PlayerController(QObject *parent);
    QMediaPlayer *instance;
    void togglePlay();
    void stop();

private slots:
    void onPlayStateChanged(bool state);

private:
    bool playState;

};

#endif // PLAYERCONTROLLER_H
