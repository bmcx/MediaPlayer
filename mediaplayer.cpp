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
    tablePlaylist = new QTableView();
    videoWidget = new QVideoWidget();

    // connections for the signals from qmediaplayey instance
    connect(player->instance, SIGNAL(positionChanged(qint64)), this, SLOT(on_positionChanged(qint64)));
    connect(player->instance, SIGNAL(durationChanged(qint64)), this, SLOT(on_durationChanged(qint64)));
    connect(player->instance, SIGNAL(currentMediaChanged(QMediaContent)), this, SLOT(on_mediaChanged()));
    connect(player->instance, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(on_mediaChanged()));
    connect(player->instance, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(on_playerStateChanged(QMediaPlayer::State)));
    connect(player->instance, SIGNAL(playbackRateChanged(qreal)), this, SLOT(on_playbackRateChanged(qreal)));
    connect(player->instance, SIGNAL(videoAvailableChanged(bool)), this, SLOT(on_videoAvailableChanged(bool)));
    connect(player->instance, SIGNAL(volumeChanged(int)), ui->sliderVolume, SLOT(setValue(int)));

    // playlist connections
    connect(player->instance->playlist(), SIGNAL(mediaChanged(int,int)), this, SLOT(on_playlistUpdated()));
    connect(player->instance->playlist(), SIGNAL(currentIndexChanged(int)), tablePlaylist, SLOT(selectRow(int)));

    // ui button connections
    connect(ui->btnPrev, SIGNAL(clicked()), player->instance->playlist(), SLOT(previous()));
    connect(ui->btnStop, SIGNAL(clicked()), player->instance, SLOT(stop()));
    connect(ui->btnNext, SIGNAL(clicked()), player->instance->playlist(), SLOT(next()));

     // ui menu action connections
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionPrevious, SIGNAL(triggered()), player->instance->playlist(), SLOT(previous()));
    connect(ui->actionStop, SIGNAL(triggered()), player->instance, SLOT(stop()));
    connect(ui->actionNext, SIGNAL(triggered()), player->instance->playlist(), SLOT(next()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    // statusbar widget setup
    ui->statusbar->addPermanentWidget(lblNowPlaying, 1);
    ui->statusbar->addPermanentWidget(lblPlaybackRate);

    // set a default value for the label
    lblPlaybackRate->setText("1.00x");

    ui->layoutMain->setAlignment(Qt::AlignCenter);
    // set the video output widget
    player->instance->setVideoOutput(videoWidget);

    // attach playlist model to view
    tablePlaylist->setModel(player->playlistModel);

    // load saved settings
    readSettings();
    // update window title from constants
    setWindowTitle(Constants::appName);
    // update volume icons after loading saved values
    update_volumeIcons();
    videoWidget->autoFillBackground();

    // set playlist table options
    tablePlaylist->hideColumn(1); // hides the file path in playlist items
    tablePlaylist->verticalHeader()->setVisible(false);
    tablePlaylist->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablePlaylist->setSelectionMode(QAbstractItemView::SingleSelection);
    tablePlaylist->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablePlaylist->horizontalHeader()->setStretchLastSection(true);

    // bind playlist ui click event to the players playlist
    connect(tablePlaylist, &QTableView::doubleClicked, [this](const QModelIndex &index){
        player->instance->playlist()->setCurrentIndex(index.row());
        player->instance->play();
    });
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
    update_volumeIcons();
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
    // if there's no files on playlist, show open file dialog
    if(player->playlistModel->rowCount() == 0){
        on_actionOpenFile_triggered();
        return;
    }
    player->togglePlayPause();
}

void MediaPlayer::on_btnShuffle_toggled(bool checked)
{
    if(checked){
        player->instance->playlist()->setPlaybackMode(QMediaPlaylist::PlaybackMode::Random);
    }else{
        player->instance->playlist()->setPlaybackMode(QMediaPlaylist::PlaybackMode::Sequential);
    }
}

void MediaPlayer::on_btnLoop_toggled(bool checked)
{
    if(checked){
        player->instance->playlist()->setPlaybackMode(QMediaPlaylist::PlaybackMode::Loop);
    }else{
        player->instance->playlist()->setPlaybackMode(QMediaPlaylist::PlaybackMode::Sequential);
    }
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

        setWindowIcon(QIcon(Constants::playIcon));
        ui->actionPlay->setIcon(QIcon(Constants::pauseIcon));
        ui->btnPlay->setIcon(QIcon(Constants::pauseIcon));
        break;
    case QMediaPlayer::PausedState:
        ui->actionPlay->setText("Play");

        setWindowIcon(QIcon(Constants::pauseIcon));
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

/// if the selected file only a audio file disable video related features
/// if file is a video, enable video related features here
void MediaPlayer::on_videoAvailableChanged(bool available)
{
    if(available){
        videoWidget->show();
        ui->bg->hide();
        ui->layoutVideo->replaceWidget(ui->bg, videoWidget);
        // a bit of a workaround to strech player widget by using vspacer
        // use setStrech to hide spacer width
        ui->layoutVideo->setStretch(0,0);
        // setting the strech of the video widget to full
        ui->layoutVideo->setStretch(1,1);

        connect(ui->actionFullscreen, SIGNAL(toggled(bool)), videoWidget, SLOT(setFullScreen(bool)));
        ui->actionFullscreen->setEnabled(true);
        ui->menuBrightness->setEnabled(true);
        ui->menuContrast->setEnabled(true);
    }else{
        ui->layoutVideo->replaceWidget(videoWidget, ui->bg);
        videoWidget->hide();
        ui->bg->show();
        disconnect(ui->actionFullscreen, SIGNAL(toggled(bool)), videoWidget, SLOT(setFullScreen(bool)));
        ui->actionFullscreen->setEnabled(false);
        ui->menuBrightness->setEnabled(false);
        ui->menuContrast->setEnabled(false);
    }
}

/// show/hide playlist
void MediaPlayer::on_btnPlaylist_toggled(bool checked)
{
    if(checked){
        ui->layoutMain->addWidget(tablePlaylist);
        ui->layoutMain->setStretch(0,7);
        ui->layoutMain->setStretch(1,3);
        tablePlaylist->show();
    }else{
        ui->layoutMain->removeWidget(tablePlaylist);
        ui->layoutMain->setStretch(0,1);
        ui->layoutMain->setStretch(1,0);
        tablePlaylist->hide();
    }
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
    // save playlist openned state
    settings.setValue("playlist_openned", ui->btnPlaylist->isChecked());
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
    // load playlist openned state
    bool playlistOpenned =settings.value("playlist_openned", true).toBool();
    ui->btnPlaylist->setChecked(playlistOpenned);
    on_btnPlaylist_toggled(playlistOpenned);
    settings.endGroup();
}

/// overrides the default close event to save changes
void MediaPlayer::closeEvent (QCloseEvent *event){
    // store changed settings
    writeSettings();
}

/// update volume icons with current state of the player
void MediaPlayer::update_volumeIcons()
{
    int volume = player->instance->volume();
    // check if the player is muted or not
    if(ui->actionMute->isChecked()){
        ui->lblVolumeIcon->setPixmap(QPixmap(Constants::volumeMuteIcon));
    }else{
        if(volume > 50){
            ui->lblVolumeIcon->setPixmap(QPixmap(Constants::volumeUpIcon));
        }else if(volume == 0){
            ui->lblVolumeIcon->setPixmap(QPixmap(Constants::volumeMuteIcon));
        }else{
            ui->lblVolumeIcon->setPixmap(QPixmap(Constants::volumeDownIcon));
        }
    }
}

// menu actions

/// open files and add them to playlist
void MediaPlayer::on_actionOpenFile_triggered()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this, "Open Files", lastPath, Constants::acceptedFileTypes);
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

/// handles muted or unmuted state
void MediaPlayer::on_actionMute_toggled(bool muted)
{
    player->instance->setMuted(muted);
    update_volumeIcons();
}

/// save a playlist to a local file
void MediaPlayer::on_actionSavePlaylistToFile_triggered()
{
    QString filename =QFileDialog::getSaveFileName(this,"Save file", lastPath, Constants::acceptedPlaylistFileTypes);
    player->instance->playlist()->save(QUrl::fromLocalFile(filename),"m3u");
}

/// open a saved playlist from a local file
void MediaPlayer::on_actionOpenPlaylist_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open a File", lastPath, Constants::acceptedPlaylistFileTypes);
    player->laodPlaylist(filename);
}

/// video brighness increase
void MediaPlayer::on_actionBIncrease_triggered()
{
    int brightness = videoWidget->brightness();
    if(brightness<100) videoWidget->setBrightness(brightness+5);
     ui->menuBrightness->setTitle(QString("Brightness: %1\%").arg(brightness+5));
}

/// resets video brightness to defaults
void MediaPlayer::on_actionBReset_triggered()
{
    videoWidget->setBrightness(0);
     ui->menuBrightness->setTitle("Brightness");
}

/// decrease video brightness
void MediaPlayer::on_actionBDecrease_triggered()
{
    int brightness = videoWidget->brightness();
    if(brightness>-100) videoWidget->setBrightness(brightness-5);
     ui->menuBrightness->setTitle(QString("Brightness: %1\%").arg(brightness+5));
}

/// increase video contrast
void MediaPlayer::on_actionCIncrease_triggered()
{
    int contrast = videoWidget->contrast();
    if(contrast<100) videoWidget->setContrast(contrast+5);
    ui->menuContrast->setTitle(QString("Contrast: %1\%").arg(contrast+5));
}

/// reset video contrast
void MediaPlayer::on_actionCReset_triggered()
{
    videoWidget->setContrast(0);
    ui->menuContrast->setTitle("Contrast");
}

/// decrease video contrast
void MediaPlayer::on_actionCDecrease_triggered()
{
    int contrast = videoWidget->contrast();
    if(contrast>-100) videoWidget->setContrast(contrast-5);
    ui->menuContrast->setTitle(QString("Contrast: %1\%").arg(contrast-5));
}

/// confirm before clearing the playlist
void MediaPlayer::on_actionClearPlaylist_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Are you sure?", "Current playlist will be cleared.",
                                  QMessageBox::Ok|QMessageBox::Cancel);
    if (reply == QMessageBox::Ok) {
        player->clearPlaylist();
        tablePlaylist->update();
    }
}


void MediaPlayer::on_actionAbout_triggered()
{
    QMessageBox::about(this,Constants::appName, "A simple media player inspired by VLC.\nCreated using Qt framework for C++\nEnjoy!\n");
}

void MediaPlayer::on_actionHelp_triggered()
{
    // open help?!
}

