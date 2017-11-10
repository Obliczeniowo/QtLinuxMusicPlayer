#ifndef QVOLUMEVIEW_H
#define QVOLUMEVIEW_H

#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsScene>

#include <QMediaPlayer>
#include <QAudioProbe>

#include "qvolumecontrol.h"

class QVolumeView : public QGraphicsView
{
    Q_OBJECT

    QMediaPlayer *player;

    QAudioProbe *probe;

    QGraphicsScene      *scene;
    QGraphicsRectItem   *leftVolume;
    QGraphicsRectItem   *rightVolume;

    QVolumeControl      *volumeControl;

public:
    explicit QVolumeView(QMediaPlayer *player, QVolumeControl *volumeControl, QWidget *parent = nullptr);

signals:

public slots:
    void processBuffer(QAudioBuffer);
};

#endif // QVOLUMEVIEW_H
