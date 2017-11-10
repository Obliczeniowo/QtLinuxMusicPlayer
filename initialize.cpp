#include "initialize.h"

Initialize::Initialize()
{
    applicationPath = QDir::current();
    iniFileName = "initialize.ini";

    options["currentPath"] = QDir::homePath();
    options["volume"] = "100";
}

void Initialize::loadData(){
    QFile iniFile(applicationPath.filePath(iniFileName));

    if(iniFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream readIni(&iniFile);
        QString iniLine;
        while(!readIni.atEnd()){
            iniLine = readIni.readLine();
            QStringList iniOption = iniLine.split("=",QString::SkipEmptyParts);

            if(iniOption.count() == 2){
                QString key = iniOption.at(0).trimmed();
                QString value = iniOption.at(1).trimmed();
                if(key.length() && value.length() && options.find(key) != options.end()){
                    options[key] = value;
                }
            }
        }
        iniFile.close();

        QMap<QString, QString>::const_iterator finded = options.find("currentPath");

        if(finded != options.end()){
            QDir dir(finded.value());
            if(!dir.exists()){
                options["currentPath"] = QDir::homePath();
            }
        }

        finded = options.find("volume");
        if(finded != options.end()){
            bool ok = false;
            int volume = finded.value().toInt(&ok, 10);
            if(ok){
                options["volume"] = QString().setNum(volume > -1 ? (volume < 101 ? volume : 100) : 0);
            }else{
                options["volume"] = "100";
            }
        }


    }else{
        options["currentPath"] = QDir::homePath();
        options["volume"] = "100";
    }
}

void Initialize::saveData(){
    QFile iniFile(applicationPath.filePath(iniFileName));

    if(iniFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream writeIni(&iniFile);

        for(QMap<QString, QString>::iterator option = options.begin(); option != options.end(); option++){
            writeIni << option.key() << "=" << option.value() << "\n";
        }

        iniFile.close();
    }
}

QString Initialize::getOption(const QString &key) const {
    return options[key];
}

int Initialize::getOptionInt(const QString &key) const {
    return options[key].toInt();
}

void Initialize::setOption(const QString &key, QString value){
    if(key.length())
        options[key] = value;
}

void Initialize::setOption(const QString &key, int value){
    if(key.length())
        options[key] = QString().setNum(value);
}
