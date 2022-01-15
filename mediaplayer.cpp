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

//    ui->statusbar->addWidget(widget,1);
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
    qDebug() << duration;
    ui->sliderProgress->setMaximum(duration);
    ui->lblDuration->setText(Utils::formatDuration(duration));
}


void MediaPlayer::on_btnPlay_clicked()
{
    player->togglePlayPause();
}


void MediaPlayer::on_actionOpenFile_triggered()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this, "Open a File","",Constants::acceptedFileTypes);
    player->addToPlaylist(filenames);
}

