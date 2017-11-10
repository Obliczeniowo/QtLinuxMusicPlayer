#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMediaPlayer>
#include <QAudioProbe>
#include <QMediaMetaData>

#include <QMessageBox>
#include <QAudioBuffer>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMediaPlaylist>
#include <QFileDialog>

#include <QtCore>
#include <QtGui>

#include "initialize.h"

#include "qpositionwidget.h"
#include "qvolumecontrol.h"
#include "qvolumeview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Initialize ini;

    QMediaPlayer *player;

    QMediaPlaylist *playlist;

    QVolumeControl  *volumeControl;
    QVolumeView     *volumeView;

    QDir wndPath;

    QPositionWidget *position;

    QStringList fileExtensionsList = {"mp3", "ogg", "wma"};

    void loadFileList(QStringList &fileList);

    void loadFileListFromFile();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void onChange(QMediaPlayer::State state);

    void onPositionChange(qint64 position);
    void onDurationChange(qint64 duration);

private slots:
    void on_duration_windowIconTextChanged(const QString &iconText);

    void on_play_clicked(bool checked);

    void on_play_clicked();

    void on_next_clicked();

    void on_previous_clicked();

    void on_forward5s_clicked();

    void on_back5s_clicked();

    void on_tracksList_doubleClicked(const QModelIndex &index);

    void onCurrentIndexChanged(int position);

    void on_loop_clicked(bool checked);

    void on_o_triggered();

    void on_actionAuthor_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
