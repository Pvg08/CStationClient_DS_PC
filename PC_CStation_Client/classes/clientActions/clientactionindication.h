#ifndef CLIENTACTIONINDICATION_H
#define CLIENTACTIONINDICATION_H

#include "../clientaction.h"

class ClientActionIndication : public ClientAction
{
    Q_OBJECT
public:
    ClientActionIndication(AbstractServer *parent);
    virtual QString getDescriptionString();
    virtual bool runAction();
};

#endif // CLIENTACTIONINDICATION_H
