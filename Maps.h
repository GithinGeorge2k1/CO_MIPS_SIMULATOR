#ifndef MAPS_H
#define MAPS_H
#include <QMap>
#include <QPair>

class Maps{
    private:
        static Maps* KeyValueMap;
        Maps();
    public:
        static QMap<QString,QPair<int,int>> Commands;
        static QMap<QString,uint> Registers;
        static Maps* getInstance();
};


#endif // MAPS_H
