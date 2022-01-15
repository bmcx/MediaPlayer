#include "playercontroller.h"

PlayerController::PlayerController(QObject *parent)
{
    instance = new QMediaPlayer(parent);
    playlist = new QMediaPlaylist(instance);
    instance->setPlaylist(playlist);
}

void PlayerController::togglePlayPause()
{
    switch (instance->state()) {
    case  QMediaPlayer::PlayingState:
        instance->pause();
        break;
    case QMediaPlayer::PausedState:
    default:
        instance->play();
    }

    qDebug() << instance->state();
    qDebug() << instance->errorString();

}

void PlayerController::stop()
{
    instance->stop();
}

void PlayerController::addToPlaylist(QStringList paths)
{
    for(const QString & path: paths){
        playlist->addMedia(QMediaContent(QUrl::fromLocalFile(path)));
    }

}

