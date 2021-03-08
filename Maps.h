#ifndef MAPS_H
#define MAPS_H
#include <QMap>
#include <QPair>

class Maps{

    Maps();
public:
    static QMap<QString,QPair<int,int>> Commands;
    static QMap<QString,uint> Registers;
    static Maps* KeyValueMap;
    static Maps* getInstance();
};


#endif // MAPS_H
