#include "Maps.h"
#include<QFile>
#include <QTextStream>
#include <QDebug>

QMap<QString,uint> Maps::Commands;
QMap<QString,uint> Maps::Registers;
Maps* KeyValueMap=NULL;
Maps::Maps()
{
    QString path=":/MapCodes/Commands.csv";
    QFile commands(path);
    QTextStream fin1(&commands);
    while(!fin1.atEnd())
    {
        QString line=fin1.readLine().simplified();
        QStringList keyvaluepair=line.split(",0x");
        Commands.insert(keyvaluepair.at(0), keyvaluepair.at(1).toInt());
    }
    commands.close();
    path=":/MapCodes/Registers.csv";
    QFile registers(path);
    QTextStream fin2 (&registers);
    while(!fin2.atEnd())
    {
        QString line=fin2.readLine().simplified();
        QStringList keyvaluepair=line.split(",o=0x");
        Registers.insert(keyvaluepair.at(0), keyvaluepair.at(1).toUInt());
    }
    registers.close();
}
Maps* Maps::getInstance()
{
    if(KeyValueMap==NULL)
        KeyValueMap=new Maps();
    return KeyValueMap;
}




