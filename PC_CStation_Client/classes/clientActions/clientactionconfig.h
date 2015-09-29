#ifndef CLIENTACTIONCONFIG_H
#define CLIENTACTIONCONFIG_H

#include "../clientaction.h"

class ClientActionConfig : public ClientAction
{
    Q_OBJECT
public:
    ClientActionConfig(AbstractServer *parent);
    virtual QString getDescriptionString();
    virtual bool runAction();
};

#endif // CLIENTACTIONCONFIG_H
