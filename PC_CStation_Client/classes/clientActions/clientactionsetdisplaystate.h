#ifndef CLIENTACTIONSETDISPLAYSTATE_H
#define CLIENTACTIONSETDISPLAYSTATE_H

#define WINVER 0x0601
#define _WIN32_WINNT 0x0601

#include <Windows.h>

#include "../clientaction.h"

class ClientActionSetDisplayState : public ClientAction
{
    Q_OBJECT
public:
    ClientActionSetDisplayState(AbstractServer *parent);
    virtual bool runAction();
};

#endif // CLIENTACTIONSETDISPLAYSTATE_H
