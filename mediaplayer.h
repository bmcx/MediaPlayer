#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QMainWindow>
#include <QVideoWidget>
#include <QDebug>
#include <QtGui>
#include <QLabel>
#include <QStandardItemModel>
#include <QCloseEvent>
#include <QTableView>
#include <QHeaderView>

#include "controllers/playercontroller.h"
#include "constants.h"
#include "utils.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MediaPlayer; }
QT_END_NAMESPACE

class MediaPlayer : public QMainWindow
{
    Q_OBJECT

public:
    MediaPlayer(QWidget *parent = nullptr);
    ~MediaPlayer();

private slots:
    void on_sliderProgress_sliderMoved(int position);
    void on_sliderVolume_sliderMoved(int position);
    void on_positionChanged(qint64 position);
    void on_durationChanged(qint64 position);
    void on_mediaChanged();
    void on_btnPlay_clicked();
    void on_actionOpenFile_triggered();
    void on_playlistUpdated();
    void on_playbackRateChanged(qreal rate);
    void on_btnShuffle_toggled(bool checked);
    void on_btnLoop_toggled(bool checked);
    void on_playerStateChanged(QMediaPlayer::State state);
    void on_videoAvailableChanged(bool available);
    void on_btnPlaylist_toggled(bool checked);

    void on_actionPSFaster_triggered();
    void on_actionPSNormal_triggered();
    void on_actionPSSlower_triggered();
    void on_actionJumpForward_triggered();
    void on_actionJumpBackward_triggered();
    void on_actionPlay_triggered();
    void on_actionIncreaseVolume_triggered();
    void on_actionDecreaseVolume_triggered();
    void on_actionMute_toggled(bool value);
    void on_actionSavePlaylistToFile_triggered();
    void on_actionOpenPlaylist_triggered();
    void on_actionBIncrease_triggered();
    void on_actionBReset_triggered();
    void on_actionBDecrease_triggered();
    void on_actionCIncrease_triggered();
    void on_actionCReset_triggered();
    void on_actionCDecrease_triggered();

    void on_actionClearPlaylist_triggered();

private:
    Ui::MediaPlayer *ui;
    PlayerController *player;
    QVideoWidget *videoWidget;
    QString lastPath;
    QLabel *lblNowPlaying;
    QLabel *lblPlaybackRate;
    QTableView *tablePlaylist;

    void readSettings();
    void writeSettings();
    void closeEvent(QCloseEvent *event);
    void update_volumeIcons();


};
#endif // MEDIAPLAYER_H
