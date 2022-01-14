#include "mediaplayer.h"
#include "ui_mediaplayer.h"

MediaPlayer::MediaPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MediaPlayer)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);

    connect(player, &QMediaPlayer::positionChanged, this, &MediaPlayer::on_positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MediaPlayer::on_durationChanged);

//    ui->statusbar->addWidget(widget,1);
}

MediaPlayer::~MediaPlayer()
{
    delete ui;
}


void MediaPlayer::on_sliderProgress_sliderMoved(int position)
{
    player->setPosition(position);
}


void MediaPlayer::on_sliderVolume_sliderMoved(int position)
{
    player->setVolume(position);
}


void MediaPlayer::on_pushButton_clicked()
{
    QString vidstring = QFileDialog::getOpenFileName(this, "Select video file", QDir::homePath());
    player->setMedia(QUrl::fromLocalFile(vidstring));
    player->play();
    qDebug() << player->errorString();
}


void MediaPlayer::on_pushButton_2_clicked()
{
    player->stop();
}

void MediaPlayer::on_positionChanged(qint64 position)
{
    ui->sliderProgress->setValue(position);
}

void MediaPlayer::on_durationChanged(qint64 position)
{
    ui->sliderProgress->setMaximum(position);
}

