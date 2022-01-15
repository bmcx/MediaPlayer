#include "mediaplayer.h"
#include "ui_mediaplayer.h"

MediaPlayer::MediaPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MediaPlayer)
{
    ui->setupUi(this);
    player = new PlayerController(this);

    connect(player->instance, SIGNAL(positionChanged(qint64)), this, SLOT(on_positionChanged(qint64)));
    connect(player->instance, SIGNAL(durationChanged(qint64)), this, SLOT(on_durationChanged(qint64)));
    connect(player->instance, SIGNAL(currentMediaChanged(QMediaContent)), this, SLOT(on_mediaChanged()));

    connect(player->instance->playlist(), SIGNAL(mediaChanged(int,int)), this, SLOT(on_playlistUpdated()));

    //    ui->statusbar->addWidget(widget,1);

    // ui button connections
    connect(ui->btnPrev, SIGNAL(clicked()), player->instance->playlist(), SLOT(previous()));
    connect(ui->btnNext, SIGNAL(clicked()), player->instance->playlist(), SLOT(next()));

    // set default path to home dir
    lastPath = QDir::homePath();
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
        qDebug() << player->instance->playlist()->objectName();
    }

}

void MediaPlayer::on_actionOpenFile_triggered()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this, "Open a File", lastPath, Constants::acceptedFileTypes);
    if(!filenames.isEmpty()){
        player->addToPlaylist(filenames);
        // save last openned path
        lastPath = QFileInfo(filenames[0]).path();
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

