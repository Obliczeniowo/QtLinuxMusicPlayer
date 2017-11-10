#include "qvolumeview.h"

QVolumeView::QVolumeView(QMediaPlayer *player, QVolumeControl *volumeControl,QWidget *parent) : QGraphicsView(parent), player(player), volumeControl(volumeControl)
{
    scene = new QGraphicsScene(0, 0, 200, 70, this);
    this->setScene(scene);
    scene->setBackgroundBrush(QBrush(QColor(225,225,225,255)));

    QLinearGradient volumeGradient(0, 0, 200, 0);
    volumeGradient.setColorAt(0, QColor::fromRgbF(0, 1, 0, 1));
    volumeGradient.setColorAt(0.5, QColor::fromRgbF(1, 1, 0, 1));
    volumeGradient.setColorAt(1, QColor::fromRgbF(1, 0, 0, 1));

    QBrush gradientBrush(volumeGradient);

    QLinearGradient volumeBackgroundGradient(0, 0, 200, 0);

    volumeBackgroundGradient.setColorAt(0, QColor::fromRgbF(0, 0.3, 0, 1));
    volumeBackgroundGradient.setColorAt(0.5, QColor::fromRgbF(0.3, 0.3, 0, 1));
    volumeBackgroundGradient.setColorAt(1, QColor::fromRgbF(0.3, 0, 0, 1));

    QPen grayPen(QColor(235, 235, 235));

    scene->addRect(0, 10, 200, 20, grayPen, volumeBackgroundGradient);
    leftVolume = scene->addRect(0, 10, 1, 20, grayPen, gradientBrush);
    scene->addRect(0, 40, 200, 20, grayPen, volumeBackgroundGradient);
    rightVolume = scene->addRect(0, 40, 1, 20, grayPen, gradientBrush);

    probe = new QAudioProbe;

    connect(probe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(processBuffer(QAudioBuffer)));

    probe->setSource(player);
}

void QVolumeView::processBuffer(QAudioBuffer ab){
    //QAudioBuffer.StereoFrame();

    quint8 channels = ab.format().channelCount();
    quint8 sample = 8 * ab.format().bytesPerFrame() / channels;

    if(sample == 16){
        const qint16 *data = ab.constData<qint16>();

        quint64 frames = ab.frameCount() * channels;

        qint64 left = 0;
        qint64 right = 0;

        qint16 *end = (qint16*)data + frames;

        for(qint16* i = (qint16*)data; i < end; i+= channels ){
            if(abs(*i) > left)
                left = abs(*i);
            if(abs(*(i + channels - 1)) > right){
                right = abs(*(i + channels - 1));
            }
        }

        if(channels == 1){
            left = left > right ? left : right;
            right = left;
        }

        qreal rleft = QAudio::convertVolume(qreal(left) / ((1 << sample) / 2),
                                                        QAudio::LinearVolumeScale,
                                                        QAudio::LogarithmicVolumeScale
                                            );
        qreal rright = QAudio::convertVolume(qreal(right) / ((1 << sample) / 2),
                                                        QAudio::LinearVolumeScale,
                                                        QAudio::LogarithmicVolumeScale
                                            );

        QRectF r = rightVolume->rect();
        float volume = qreal(volumeControl->volume()) / 100.;
        r.setWidth(rleft * volume * scene->width());
        rightVolume->setRect(r);

        QRectF l = leftVolume->rect();
        l.setWidth(rright * volume * scene->width());
        leftVolume->setRect(l);
    }else if(sample == 8){
        const qint8 *data = ab.constData<qint8>();

        quint64 frames = ab.frameCount() * channels;

        qint64 left = 0;
        qint64 right = 0;

        qint8 *end = (qint8*)data + frames;

        for(qint8* i = (qint8*)data; i < end; i+= channels ){
            if(abs(*i) > left)
                left = abs(*i);
            if(abs(*(i + channels - 1)) > right){
                right = abs(*(i + channels - 1));
            }
        }

        if(channels != 1){
            left = left > right ? left : right;
            right = left;
        }

        qreal rleft = QAudio::convertVolume(qreal(left) / ((1 << sample) / 2),
                                                        QAudio::LinearVolumeScale,
                                                        QAudio::LogarithmicVolumeScale
                                            );
        qreal rright = QAudio::convertVolume(qreal(right) / ((1 << sample) / 2),
                                                        QAudio::LinearVolumeScale,
                                                        QAudio::LogarithmicVolumeScale
                                            );

        QRectF r = rightVolume->rect();
        float volume = qreal(volumeControl->volume()) / 100.;
        r.setWidth(rleft * volume * scene->width());
        rightVolume->setRect(r);

        QRectF l = leftVolume->rect();
        l.setWidth(rright * volume * scene->width());
    }else if(sample == 32){
        const qint32 *data = ab.constData<qint32>();

        quint32 frames = ab.frameCount() * channels;

        qint32 left = 0;
        qint32 right = 0;

        qint32 *end = (qint32*)data + frames;

        for(qint32* i = (qint32*)data; i < end; i+= channels ){
            if(abs(*i) > left)
                left = abs(*i);
            if(abs(*(i + channels - 1)) > right){
                right = abs(*(i + channels - 1));
            }
        }

        if(channels == 1){
            left = left > right ? left : right;
            right = left;
        }

        qreal rleft = QAudio::convertVolume(qreal(left) / ((1ull << sample) / 2),
                                                        QAudio::LinearVolumeScale,
                                                        QAudio::LogarithmicVolumeScale
                                            );
        qreal rright = QAudio::convertVolume(qreal(right) / ((1ull << sample) / 2),
                                                        QAudio::LinearVolumeScale,
                                                        QAudio::LogarithmicVolumeScale
                                            );

        QRectF r = rightVolume->rect();
        float volume = qreal(volumeControl->volume()) / 100.;
        r.setWidth(rleft * volume * scene->width());
        rightVolume->setRect(r);

        QRectF l = leftVolume->rect();
        l.setWidth(rright * volume * scene->width());
        leftVolume->setRect(l);
    }

}
