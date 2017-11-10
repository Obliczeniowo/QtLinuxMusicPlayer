#include "qpositionwidget.h"

QString getStringTime(qint64 time){
    return QString("%1:%2:%3").arg(time / 3600000, 2, 10, QLatin1Char('0'))
                       .arg((time / 60000) % 60, 2, 10, QLatin1Char('0'))
                       .arg((time / 1000) % 60, 2, 10, QLatin1Char('0'));
}

QPositionWidget::QPositionWidget(QMediaPlayer *player, QWidget *parent) : QWidget(parent), player(player)
{
    penGray = QPen(QColor(220, 220, 220));
    brBackground = QBrush(QColor(200,200,200),Qt::SolidPattern);
    brScrollPosition = QBrush(QColor(150,150,150),Qt::SolidPattern);
    brTrackPosition = QBrush(QColor(255,255,255,150),Qt::SolidPattern);

    this->setAutoFillBackground(true);
    this->setMouseTracking(true);

    //this->setToolTipDuration(4000);

    QObject::connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChanged(qint64)));
}

QPositionWidget::~QPositionWidget(){

}

void QPositionWidget::paintEvent(QPaintEvent *event){
    QWidget::paintEvent(event);

    QPainter painter(this);

    painter.setPen(QColor(0,0,0));

    painter.setBrush(brBackground);

    QRect r = geometry();

    painter.drawRect(0, 0, r.width() - 1, r.height() - 1);

    quint64 duration = player->duration();
    quint64 position = player->position();

    if(duration){
        painter.setBrush(brScrollPosition);
        qreal wsp = qreal(position) / duration;
        painter.drawRect(0, 0, geometry().width() * wsp, r.height() - 1);
    }

    painter.setBrush(brTrackPosition);
    QPoint p = this->mapFromGlobal(QCursor::pos());
    if(p.x() > -1 && p.x() < this->geometry().width() && p.y() > -1 && p.y() < this->geometry().height()){
        painter.drawRect(0, 0, p.x(), r.height() - 1);
    }
}

void QPositionWidget::mousePressEvent(QMouseEvent *event){
    QWidget::mousePressEvent(event);

    if(geometry().width()){
        qreal wsp = qreal(event->x()) / geometry().width();
        player->setPosition(player->duration() * wsp);
    }
}

void QPositionWidget::mouseMoveEvent(QMouseEvent *event){
    if(this->width()){
        QPoint p = this->mapFromGlobal(QCursor::pos());

        //this->setToolTip(getStringTime(player->position()));
        QToolTip::showText( QCursor::pos(), getStringTime(player->duration() * qreal(p.x()) / this->width()));
        this->repaint();
    }
}

void QPositionWidget::leaveEvent(QEvent *event){
    this->repaint();
}

void QPositionWidget::resizeWindow(int width, int height){

    QRect rect = this->geometry();
    rect.setWidth(width);
    rect.setHeight(height);

    this->setGeometry(rect);

    this->repaint();
}

void QPositionWidget::onPositionChanged(qint64 position){
    this->repaint();
}
