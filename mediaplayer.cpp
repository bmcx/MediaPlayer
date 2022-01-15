#include "mediaplayer.h"
#include "ui_mediaplayer.h"

MediaPlayer::MediaPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MediaPlayer)
{
    ui->setupUi(this);
    player = new PlayerController(this);
    lblNowPlaying = new QLabel(this);

    // connections for the signals from qmediaplayey instance
    connect(player->instance, SIGNAL(positionChanged(qint64)), this, SLOT(on_positionChanged(qint64)));
    connect(player->instance, SIGNAL(durationChanged(qint64)), this, SLOT(on_durationChanged(qint64)));
    connect(player->instance, SIGNAL(currentMediaChanged(QMediaContent)), this, SLOT(on_mediaChanged()));
    connect(player->instance, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(on_mediaChanged()));
    connect(player->instance, SIGNAL(volumeChanged(int)), ui->sliderVolume, SLOT(setValue(int)));

    // playlist connections
    connect(player->instance->playlist(), SIGNAL(mediaChanged(int,int)), this, SLOT(on_playlistUpdated()));

    // ui button connections
    connect(ui->btnPrev, SIGNAL(clicked()), player->instance->playlist(), SLOT(previous()));
    connect(ui->btnStop, SIGNAL(clicked()), player->instance, SLOT(stop()));
    connect(ui->btnNext, SIGNAL(clicked()), player->instance->playlist(), SLOT(next()));

    // set default path to home dir
    lastPath = QDir::homePath();

    // statusbar widget setup
    ui->statusbar->addPermanentWidget(lblNowPlaying, 1);


//    playlistModel = new QStringListModel(this);

}

MediaPlayer::~MediaPlayer()
{
    delete ui;
}

void MediaPlayer::on_sliderProgress_sliderMoved(int position)
{
    player->instance->setPosition(position);
}

void MediaPlayer::on_sliderVolume_sliderMoved(int position)
{
    player->instance->setVolume(position);
}

void MediaPlayer::on_positionChanged(qint64 position)
{
//    qDebug() << position;
    ui->sliderProgress->setValue(position);
    ui->lblCurrentTime->setText(Utils::formatDuration(position));
}

void MediaPlayer::on_durationChanged(qint64 duration)
{
    qDebug() << "Loaded media duration: "<< duration;
    ui->sliderProgress->setMaximum(duration);
    ui->lblDuration->setText(Utils::formatDuration(duration));
}

void MediaPlayer::on_mediaChanged()
{
    if(!player->instance->currentMedia().isNull()){
        qDebug() << player->instance->currentMedia().canonicalUrl().fileName();
        lblNowPlaying->setText(player->instance->currentMedia().canonicalUrl().fileName());
    }else{
        lblNowPlaying->clear();
    }

}

void MediaPlayer::on_playlistUpdated()
{
    qDebug() << "Media count: " << player->instance->playlist()->mediaCount();
}

// control buttons

void MediaPlayer::on_btnPlay_clicked()
{
    player->togglePlayPause();
}

void MediaPlayer::on_btnShuffle_toggled(bool checked)
{
    qDebug() << checked;
}

// menu actions

void MediaPlayer::on_actionOpenFile_triggered()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this, "Open a File", lastPath, Constants::acceptedFileTypes);
    if(!filenames.isEmpty()){
        player->addToPlaylist(filenames);
        // save last openned path
        lastPath = QFileInfo(filenames[0]).path();
    }
}

/// increase playback rate by incrementing current rate by 0.1
void MediaPlayer::on_actionPSFaster_triggered()
{
    qreal rate = player->instance->playbackRate();
    player->instance->setPlaybackRate(rate + 0.1);
}

/// resets the playback rate to normal; rate(1)
void MediaPlayer::on_actionPSNormal_triggered()
{
    player->instance->setPlaybackRate(1);
}

/// decrease playback rate by decrementing 0.1 from current rate
void MediaPlayer::on_actionPSSlower_triggered()
{
    qreal rate = player->instance->playbackRate();
    player->instance->setPlaybackRate(rate - 0.1);
}

/// jump forward 10 seconds
void MediaPlayer::on_actionJumpForwards_triggered()
{
    player->changePositionBy(10);
}

/// jump backward 10 seconds
void MediaPlayer::on_actionJumpBackward_triggered()
{
    player->changePositionBy(-10);
}
