#include "playercontroller.h"

PlayerController::PlayerController(QObject *parent)
{
    instance = new QMediaPlayer(parent);
}

void PlayerController::togglePlay()
{
    //    QString vidstring = QFileDialog::getOpenFileName(this, "Select video file", QDir::homePath());
    qDebug() << instance->state();
    switch (instance->state()) {
    case  QMediaPlayer::PlayingState:
        instance->pause();
        break;
    case QMediaPlayer::PausedState:
        instance->play();
        break;
    default:
        QString vidstring = "C:\\dev\\c++\\MediaPlayer\\test.mp3";
        instance->setMedia(QUrl::fromLocalFile(vidstring));
        instance->play();
        qDebug() << instance->errorString();
    }

}

void PlayerController::stop()
{
    instance->stop();
}

void PlayerController::onPlayStateChanged(bool state)
{
    playState = state;
}
