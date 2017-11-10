#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMap>
#include <QStringList>
#include <QString>


class Initialize : public QObject
{
    Q_OBJECT

    QDir applicationPath;
    QString iniFileName;

    QMap<QString, QString> options;

public:
    Initialize();
    void loadData();
    void saveData();

    QString getOption(const QString &key) const;
    int getOptionInt(const QString &key) const;
    void setOption(const QString &key, QString value);
    void setOption(const QString &key, int value);
};

#endif // INITIALIZE_H
