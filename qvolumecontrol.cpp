#include "qvolumecontrol.h"

QVolumeControl::QVolumeControl(QMediaPlayer *player, QWidget *parent) : QWidget(parent), player(player)
{
    slVolume = new QSlider(this);
    slVolume->setOrientation(Qt::Vertical);
    slVolume->setGeometry(22, 0, 16, 70);
    slVolume->setMaximum(100);
    slVolume->setMinimum(0);
    slVolume->setValue(100);

    gvVolume = new QGraphicsView(this);
    gvVolume->setGeometry(0, 5, 21, 60);
    gvVolume->setFrameShape(QFrame::NoFrame);

    grScene = new QGraphicsScene(0, 0, gvVolume->geometry().width(), gvVolume->geometry().height(), gvVolume);
    gvVolume->setScene(grScene);

    grScene->setBackgroundBrush(QBrush(QWidget::palette().color(QWidget::backgroundRole())));

    QLinearGradient volumeLevelGradient(0, 0, 0, gvVolume->geometry().height());
    volumeLevelGradient.setColorAt(1, QColor::fromRgbF(0, 1, 0, 1));
    volumeLevelGradient.setColorAt(0.5, QColor::fromRgbF(1, 1, 0, 1));
    volumeLevelGradient.setColorAt(0, QColor::fromRgbF(1, 0, 0, 1));

    QPen grayPen(QColor(235, 235, 235), 1);
    QBrush gradientBrush(volumeLevelGradient);

    QPolygonF volumePolygon;
    volumePolygon.push_back(QPointF(0,0));
    volumePolygon.push_back(QPointF(gvVolume->geometry().width() - 4, 0));
    volumePolygon.push_back(QPointF(gvVolume->geometry().width() - 4, gvVolume->geometry().height() - 4));

    polygonGradient = grScene->addPolygon(volumePolygon, grayPen, gradientBrush);

    QObject::connect(slVolume, SIGNAL(valueChanged(int)), this, SLOT(onVolumeChanged(int)));
}

QVolumeControl::~QVolumeControl(){
    delete gvVolume;
    //delete grScene;
    delete slVolume;
}

void QVolumeControl::onVolumeChanged(int position){
    qreal linearVolume = QAudio::convertVolume(position / qreal(100.0),
                                                     QAudio::LogarithmicVolumeScale,
                                                     QAudio::LinearVolumeScale);

    player->setVolume(qRound(linearVolume * 100));

    qreal volume = position / qreal(100.);

    qreal width = gvVolume->geometry().width() - 4 - (gvVolume->geometry().width() - 4) * volume;
    qreal height = gvVolume->geometry().height() - 4 - (gvVolume->geometry().height() - 4) * volume;

    QPolygonF volumePolygon;
    volumePolygon.push_back(QPointF(width, height));
    volumePolygon.push_back(QPointF(gvVolume->geometry().width() - 4, height));
    volumePolygon.push_back(QPointF(gvVolume->geometry().width() - 4, gvVolume->geometry().height() - 4));

    polygonGradient->setPolygon(volumePolygon);
}

int QVolumeControl::volume(){
    return slVolume->value();
}

void QVolumeControl::setVolume(int volume){
    slVolume->setValue(volume);
}
