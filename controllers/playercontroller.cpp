#include "playercontroller.h"

PlayerController::PlayerController(QObject *parent)
{
    instance = new QMediaPlayer(parent);
    playlist = new QMediaPlaylist(instance);
    playlistModel = new QStandardItemModel(parent);
    playlistModel->setHorizontalHeaderLabels(QStringList()  << "Playlist" << "File Path");
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
    // if there is an error, show message box
    if(!instance->errorString().isEmpty()){
        QMessageBox::critical(nullptr, "There was an error loading the media", instance->errorString());
    }
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
        QList<QStandardItem *> items;
        items.append(new QStandardItem(QDir(path).dirName()));
        // items.append(new QStandardItem(path));
        // add duration of the media file too?
        playlist->addMedia(QMediaContent(QUrl::fromLocalFile(path)));
        playlistModel->appendRow(items);
    }
}

void PlayerController::laodPlaylist(QString path)
{
    // clear existing files from playlist
    clearPlaylist();
    // load the files to player playlist
    instance->playlist()->load(QUrl::fromLocalFile(path));
    // update the ui with items
    QFile textFile(path);
    textFile.open(QIODevice::ReadOnly);
    QTextStream textStream(&textFile);
    while (true)
    {
        QString line = textStream.readLine();
        if (line.isNull()) break;
        else{
            QList<QStandardItem *> items;
            items.append(new QStandardItem(QDir(line).dirName()));
            // items.append(new QStandardItem(line));
            // add duration of the media file too?
            playlistModel->appendRow(items);
        }
    }

}

void PlayerController::clearPlaylist()
{
    // clear existing files from playlist
    instance->playlist()->clear();
    playlistModel->clear();
    playlistModel->setHorizontalHeaderLabels(QStringList()  << "Playlist");
}

/// change the position with given seconds
void PlayerController::changePositionBy(int value)
{
    qint64 current = instance->position();
    // current is in milliseconds so add the given value
    instance->setPosition(current + (value * 1000));
}



