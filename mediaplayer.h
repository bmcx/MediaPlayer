#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QFileDialog>
#include <QVideoWidget>
#include <QLabel>
#include <QSizePolicy>
#include <QDebug>

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

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_positionChanged(qint64 position);

    void on_durationChanged(qint64 position);

private:
    Ui::MediaPlayer *ui;
    QMediaPlayer *player;
    QVideoWidget *vw;
};
#endif // MEDIAPLAYER_H
