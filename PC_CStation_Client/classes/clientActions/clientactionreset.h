#ifndef CLIENTACTIONRESET_H
#define CLIENTACTIONRESET_H

#include "../clientaction.h"

class ClientActionReset : public ClientAction
{
    Q_OBJECT
public:
    ClientActionReset(AbstractServer *parent);
    virtual QString getDescriptionString();
    virtual bool runAction();
};

#endif // CLIENTACTIONRESET_H
