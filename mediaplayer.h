#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QMainWindow>
#include <QVideoWidget>
#include <QDebug>
#include <QtGui>
#include <QStandardItemModel>

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


    void on_btnShuffle_toggled(bool checked);

private:
    Ui::MediaPlayer *ui;
    PlayerController *player;
    QVideoWidget *vw;
    QString lastPath;
};
#endif // MEDIAPLAYER_H
