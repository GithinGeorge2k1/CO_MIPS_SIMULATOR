#ifndef MAPS_H
#define MAPS_H
#include <QMap>

class Maps{
    static QMap<QString,uint> Commands;
    static QMap<QString,uint> Registers;
    static Maps* KeyValueMap;
    Maps();
public:
    static Maps* getInstance();
};


#endif // MAPS_H
