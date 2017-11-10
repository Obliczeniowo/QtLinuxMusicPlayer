#ifndef QPOSITIONWIDGET_H
#define QPOSITIONWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QPaintEvent>
#include <QPainter>
#include <QObject>
#include <QToolTip>

QString getStringTime(qint64 time);

class QPositionWidget : public QWidget
{
    Q_OBJECT

    QBrush  brBackground;
    QBrush  brScrollPosition;
    QBrush  brTrackPosition;
    QPen    penGray;

    QMediaPlayer    *player;

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
public:
    explicit QPositionWidget(QMediaPlayer *player, QWidget *parent = nullptr);
    ~QPositionWidget();

    void resizeWindow(int width, int height);
signals:

public slots:
    void onPositionChanged(qint64 position);
};

#endif // QPOSITIONWIDGET_H
