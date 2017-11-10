#ifndef QVOLUMECONTROL_H
#define QVOLUMECONTROL_H

#include <QWidget>
#include <QSlider>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QGradient>

#include <QMediaPlayer>

class QVolumeControl : public QWidget
{
    Q_OBJECT

    QGraphicsView           *gvVolume;
    QSlider                 *slVolume;

    QGraphicsScene          *grScene;
    QGraphicsPolygonItem    *polygonGradient;

    QMediaPlayer            *player;
public:
    explicit QVolumeControl(QMediaPlayer *player, QWidget *parent = nullptr);
    ~QVolumeControl();

    int volume();
    void setVolume(int volume);
signals:

public slots:
    void onVolumeChanged(int volume);
};

#endif // QVOLUMECONTROL_H
