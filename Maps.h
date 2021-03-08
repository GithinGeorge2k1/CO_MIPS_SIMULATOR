#ifndef MAPS_H
#define MAPS_H
#include <QMap>
#include <QPair>

class Maps{
    static QMap<QString,QPair<int,int>> Commands;
    static QMap<QString,uint> Registers;
    static Maps* KeyValueMap;
    Maps();
public:
    static Maps* getInstance();
};


#endif // MAPS_H
