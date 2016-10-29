#ifndef CLIENTACTIONFORECAST_H
#define CLIENTACTIONFORECAST_H

#include "../clientaction.h"

class ClientActionForecast : public ClientAction
{
    Q_OBJECT
public:
    ClientActionForecast(AbstractServer *parent);
    virtual bool runAction();
    virtual bool setParamsFromMessage(QString message);
};

#endif // CLIENTACTIONFORECAST_H
