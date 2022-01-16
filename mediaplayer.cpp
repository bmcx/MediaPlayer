#include "mediaplayer.h"
#include "ui_mediaplayer.h"

MediaPlayer::MediaPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MediaPlayer)
{
    ui->setupUi(this);
    player = new PlayerController(this);
    lblNowPlaying = new QLabel(this);
    lblPlaybackRate = new QLabel(this);

    // connections for the signals from qmediaplayey instance
    connect(player->instance, SIGNAL(positionChanged(qint64)), this, SLOT(on_positionChanged(qint64)));
    connect(player->instance, SIGNAL(durationChanged(qint64)), this, SLOT(on_durationChanged(qint64)));
    connect(player->instance, SIGNAL(currentMediaChanged(QMediaContent)), this, SLOT(on_mediaChanged()));
    connect(player->instance, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(on_mediaChanged()));
    connect(player->instance, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(on_playerStateChanged(QMediaPlayer::State)));
    connect(player->instance, SIGNAL(playbackRateChanged(qreal)), this, SLOT(on_playbackRateChanged(qreal)));
    connect(player->instance, SIGNAL(volumeChanged(int)), ui->sliderVolume, SLOT(setValue(int)));

    // playlist connections
    connect(player->instance->playlist(), SIGNAL(mediaChanged(int,int)), this, SLOT(on_playlistUpdated()));

    // ui button connections
    connect(ui->btnPrev, SIGNAL(clicked()), player->instance->playlist(), SLOT(previous()));
    connect(ui->btnStop, SIGNAL(clicked()), player->instance, SLOT(stop()));
    connect(ui->btnNext, SIGNAL(clicked()), player->instance->playlist(), SLOT(next()));

     // ui menu action connections
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionPrevious, SIGNAL(triggered()), player->instance->playlist(), SLOT(previous()));
    connect(ui->actionStop, SIGNAL(triggered()), player->instance, SLOT(stop()));
    connect(ui->actionNext, SIGNAL(triggered()), player->instance->playlist(), SLOT(next()));
    connect(ui->actionMute, SIGNAL(toggled(bool)), player->instance, SLOT(setMuted(bool)));

    // statusbar widget setup
    ui->statusbar->addPermanentWidget(lblNowPlaying, 1);
    ui->statusbar->addPermanentWidget(lblPlaybackRate);

    // set a default value for the label
    lblPlaybackRate->setText("1.00x");

    // playlistModel = new QStringListModel(this);

    // load saved settings
    readSettings();
    // update window title from constants
    setWindowTitle(Constants::appName);
}

MediaPlayer::~MediaPlayer()
{
    delete ui;
}

/// update media position when progress slider is moved
void MediaPlayer::on_sliderProgress_sliderMoved(int position)
{
    player->instance->setPosition(position);
}

/// update volume of the player instance when slider moved
void MediaPlayer::on_sliderVolume_sliderMoved(int position)
{
    player->instance->setVolume(position);
}

/// progress bar and current time label is updated
void MediaPlayer::on_positionChanged(qint64 position)
{
//    qDebug() << position;
    ui->sliderProgress->setValue(position);
    ui->lblCurrentTime->setText(Utils::formatDuration(position));
}

/// when a file is loaded to be played this will be run
/// the duration label and progress bar size will be updated
void MediaPlayer::on_durationChanged(qint64 duration)
{
    qDebug() << "Loaded media duration(ms):" << duration;
    ui->sliderProgress->setMaximum(duration);
    ui->lblDuration->setText(Utils::formatDuration(duration));
}

/// triggers ui updates when current playing media is changed.
/// Window title and statusbar label text will be updated
void MediaPlayer::on_mediaChanged()
{
    if (!player->instance->currentMedia().isNull()) {
        QString file = player->instance->currentMedia().canonicalUrl().fileName();
        qDebug() << "Now playing:" << file;
        lblNowPlaying->setText(file);
        setWindowTitle(QString("%1 - %2").arg(file).arg(Constants::appName));
    } else {
        lblNowPlaying->clear();
        setWindowTitle(Constants::appName);
    }
}

void MediaPlayer::on_playlistUpdated()
{
    qDebug() << "Media count:" << player->instance->playlist()->mediaCount();
}

/// update playback rate label on statusbar when rate chaged
void MediaPlayer::on_playbackRateChanged(qreal rate)
{
    lblPlaybackRate->setText(QString("%1x").arg(player->instance->playbackRate(),0,'f', 2, QChar('0')));
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

/// update ui according to varoius player states like plaing/paused/stopped
void MediaPlayer::on_playerStateChanged(QMediaPlayer::State state)
{
    switch (state) {
    case QMediaPlayer::PlayingState:
        ui->actionPlay->setText("Pause");
        ui->actionStop->setEnabled(true);
        ui->menuSpeed->setEnabled(true);
        ui->actionJumpForward->setEnabled(true);
        ui->actionJumpBackward->setEnabled(true);

        setWindowIcon(QIcon(Constants::pauseIcon));
        ui->actionPlay->setIcon(QIcon(Constants::pauseIcon));
        ui->btnPlay->setIcon(QIcon(Constants::pauseIcon));
        break;
    case QMediaPlayer::PausedState:
        ui->actionPlay->setText("Play");

        setWindowIcon(QIcon(Constants::playIcon));
        ui->actionPlay->setIcon(QIcon(Constants::playIcon));
        ui->btnPlay->setIcon(QIcon(Constants::playIcon));
        break;
    case QMediaPlayer::StoppedState:
        ui->actionPlay->setText("Play");
        ui->actionStop->setEnabled(false);
        ui->menuSpeed->setEnabled(false);
        ui->actionJumpForward->setEnabled(false);
        ui->actionJumpBackward->setEnabled(false);

        setWindowIcon(QIcon(Constants::appIcon));
        ui->actionPlay->setIcon(QIcon(Constants::playIcon));
        ui->btnPlay->setIcon(QIcon(Constants::playIcon));
        setWindowTitle(Constants::appName);
        break;
    }
}

// menu actions

///
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

/// jump forward 5 seconds
void MediaPlayer::on_actionJumpForward_triggered()
{
    player->changePositionBy(5);
}

/// jump backward 5 seconds
void MediaPlayer::on_actionJumpBackward_triggered()
{
    player->changePositionBy(-5);
}

/// triggers the play or pause action according to the current state
void MediaPlayer::on_actionPlay_triggered()
{
     player->togglePlayPause();
}

/// increase volume by increments of 5
void MediaPlayer::on_actionIncreaseVolume_triggered()
{
    int volume = player->instance->volume();
    player->instance->setVolume(volume+5);
}

/// decrease volume by decrements of 5
void MediaPlayer::on_actionDecreaseVolume_triggered()
{
    int volume = player->instance->volume();
    player->instance->setVolume(volume-5);
}

/// read the saved configuration for media player main window
void MediaPlayer::writeSettings()
{
    QSettings settings(Constants::creator, Constants::appName);

    settings.beginGroup("Main");
    // save current window size and position to use in next startup
    settings.setValue("window_size", size());
    settings.setValue("window_possition", pos());
    // save last openned file path
    settings.setValue("lastPath", lastPath);
    // save volume level
    settings.setValue("player_volume", player->instance->volume());
    settings.endGroup();
}

/// write changes to the settings
void MediaPlayer::readSettings()
{
    QSettings settings(Constants::creator, Constants::appName);

    settings.beginGroup("Main");
    // restore size and position from settings, if they don't exists use default values
    resize(settings.value("window_size", QSize(850, 460)).toSize());
    move(settings.value("window_possition", QPoint(200, 200)).toPoint());
    // last opened file path, if not found uses OS's homepath as the default value
    lastPath = settings.value("lastPath", QDir::homePath()).toString();
    // load the stored volume or default to 100
    player->instance->setVolume(settings.value("player_volume", 100).toInt());
    settings.endGroup();
}

/// overrides the default close event to save changes
void MediaPlayer::closeEvent (QCloseEvent *event){
    // store changed settings
    writeSettings();
}

