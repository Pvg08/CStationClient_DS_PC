#ifndef CLIENTACTIONINDICATIONSTATE_H
#define CLIENTACTIONINDICATIONSTATE_H

#include "../clientaction.h"

class ClientActionIndicationState : public ClientAction
{
    Q_OBJECT
public:
    ClientActionIndicationState(AbstractServer *parent);
    virtual bool runAction();
};

#endif // CLIENTACTIONINDICATIONSTATE_H
