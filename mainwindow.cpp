#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ini.loadData();



    wndPath = QDir::current();

    QDir::setCurrent(QDir::homePath());

    playlist = new QMediaPlaylist;

    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    QObject::connect(playlist, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged(int)));

    player = new QMediaPlayer;

    player->setPlaylist(playlist);

    QObject::connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(onChange(QMediaPlayer::State)));
    QObject::connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChange(qint64)));
    QObject::connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(onDurationChange(qint64)));

    ui->tracksList->setColumnCount(2);
    ui->tracksList->setRowCount(playlist->mediaCount());

    for(int i = 0; i < playlist->mediaCount(); i++){
        QTableWidgetItem *item = new QTableWidgetItem(playlist->media(i).canonicalUrl().fileName());
        ui->tracksList->setItem(i, 0, item);
    }

    QStringList header;

    header << "Track name" << "duration";

    ui->tracksList->setHorizontalHeaderLabels(header);
    ui->tracksList->setColumnWidth(0, 246);
    ui->tracksList->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tracksList->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->loop->setCheckable(true);
    ui->loop->setChecked(true);

    loadFileListFromFile();

    position = new QPositionWidget(player, this->ui->centralWidget);

    position->setGeometry(10, 110, 380, 10);


    volumeControl   = new QVolumeControl(player, this->ui->centralWidget);
    volumeControl->setGeometry(280, 10, 40, 80);

    volumeView = new QVolumeView(player, volumeControl, ui->centralWidget);
    volumeView->setGeometry(10, 10, 202, 72);

    volumeControl->setVolume(ini.getOptionInt("volume"));

    QObject::connect(ui->open, SIGNAL(clicked(bool)), this, SLOT(on_o_triggered()));
}

MainWindow::~MainWindow()
{
    QString filePath = this->wndPath.filePath("tracklist");

    if(playlist->mediaCount()){
        QFile file(filePath);
        QTextStream write(&file);
        write.setCodec("UTF-8");

        if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
            for(int i = 0; i < playlist->mediaCount(); i++){
                write << (playlist->media(i).canonicalUrl().path() + "\n");
            }
        }
    }

    ini.setOption("volume", volumeControl->volume());
    ini.saveData();

    delete playlist;
    delete player;
    delete ui;
}

void MainWindow::loadFileListFromFile(){
    QString filePath = this->wndPath.filePath("tracklist");

    QFile file(filePath);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream readList(&file);

        QStringList files;

        while(!readList.atEnd()){
            QString mp3path = readList.readLine();

            if(QFile::exists(mp3path)){
                QFileInfo fi(mp3path);

                if(fileExtensionsList.indexOf(QRegularExpression(fi.suffix())) != -1){
                    files.push_back(mp3path);
                }
            }
        }
        if(files.count()){
            loadFileList(files);
        }
    }
}

void MainWindow::on_duration_windowIconTextChanged(const QString &iconText)
{

}

void MainWindow::onPositionChange(qint64 position){

    this->ui->position->setText(" " + getStringTime(position));
    this->ui->timeToEnd->setText("-" + getStringTime(player->duration() - position));
}

void MainWindow::onDurationChange(qint64 duration){
    this->ui->duration->setText(" " + getStringTime(duration));

    QTableWidgetItem *item = new QTableWidgetItem(getStringTime(duration));
    ui->tracksList->setItem(playlist->currentIndex(), 1, item);
}

void MainWindow::on_play_clicked(bool checked)
{

}

void MainWindow::onChange(QMediaPlayer::State state){
    switch(state){
        case QMediaPlayer::StoppedState:
            {
                ui->play->setText(QString("▶"));
            }
            break;
    }
}


void MainWindow::on_play_clicked()
{
    if(!playlist->mediaCount())
        return ;
    if(player->state() == QMediaPlayer::PlayingState){
        player->pause();
        ui->play->setText(QString("▶"));
    }else{
        player->play();
        ui->play->setText(QString("⏸"));
    }
}


void MainWindow::on_next_clicked()
{
    playlist->next();
}

void MainWindow::on_previous_clicked()
{
    playlist->previous();
}

void MainWindow::on_forward5s_clicked()
{
    player->setPosition(player->position() + 5000);
}

void MainWindow::on_back5s_clicked()
{
    player->setPosition(player->position() > 5000 ? player->position() - 5000 : 0);
}

void MainWindow::loadFileList(QStringList &fileList){
    if(player->state() == QMediaPlayer::PlayingState)
        on_play_clicked();
    this->playlist->clear();

    ui->tracksList->setRowCount(fileList.count());

    for(int i = 0; i < fileList.count(); i++){
        QTableWidgetItem *item = new QTableWidgetItem(QFileInfo(fileList.at(i)).baseName());
        ui->tracksList->setItem(i, 0, item);
        item = new QTableWidgetItem("--:--:--");
        ui->tracksList->setItem(i, 1, item);
    }

    foreach(QString pathToFile, fileList){
        playlist->addMedia(QUrl::fromLocalFile(pathToFile));
    }

    for(int i = 0; i < playlist->mediaCount() + 1; i++){
        playlist->next();
    }

    on_play_clicked();
}

void MainWindow::onCurrentIndexChanged(int position){
    ui->tracksList->selectRow(position);
}

void MainWindow::on_tracksList_doubleClicked(const QModelIndex &index)
{
    playlist->setCurrentIndex(index.row());
    ui->tracksList->selectRow(index.row());
}



void MainWindow::on_loop_clicked(bool checked)
{
    ui->loop->setChecked(checked);

    if(checked){
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
    }else{
        playlist->setPlaybackMode(QMediaPlaylist::Sequential);
    }
}

void MainWindow::on_o_triggered()
{
    QString path = ini.getOption("currentPath");
    QStringList list = QFileDialog::getOpenFileNames(this, "Open mp3 files", path, "Audio file (*." + fileExtensionsList.join(" *.").trimmed() + ")" );
    if(list.count()){
        QFileInfo fi(list[0]);
        ini.setOption("currentPath", fi.absolutePath());

        loadFileList(list);
    }
}

void MainWindow::on_actionAuthor_triggered()
{
    QMessageBox msg(this);

    msg.setText("Author: Krzysztof Zajączkowski\nPage: http://www.obliczeniowo.com.pl/896");

    msg.exec();
}
