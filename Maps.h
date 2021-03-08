#ifndef MAPS_H
#define MAPS_H
#include <QMap>

class Maps{
    static QMap<QString,int> Commands;
    static QMap<QString,int> Registers;
public:
    Maps();
};


#endif // MAPS_H
