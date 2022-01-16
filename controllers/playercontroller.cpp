#include "playercontroller.h"

PlayerController::PlayerController(QObject *parent)
{
    instance = new QMediaPlayer(parent);
    playlist = new QMediaPlaylist(instance);
    playlistItems = new  QStringList();
    instance->setPlaylist(playlist);
    // set initial playback rate to 1
    instance->setPlaybackRate(1);
}

/// toggle the play/pause state accordingly
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

    qDebug() << "Player state:" << instance->state();
    qDebug() << "Player errors:" << instance->errorString();

}

/// stops the player playback
void PlayerController::stop()
{
    instance->stop();
}

/// add given paths to the current playlist
void PlayerController::addToPlaylist(QStringList paths)
{
    for(const QString & path: paths){
        playlist->addMedia(QMediaContent(QUrl::fromLocalFile(path)));
        playlistItems->append(path);
    }
}

/// change the position with given seconds
void PlayerController::changePositionBy(int value)
{
    qint64 current = instance->position();
    // current is in milliseconds so add the given value
    instance->setPosition(current + (value * 1000));
}

