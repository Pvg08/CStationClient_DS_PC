#ifndef CLIENTACTIONLCD_H
#define CLIENTACTIONLCD_H

#include "../clientaction.h"

class ClientActionLcd : public ClientAction
{
    Q_OBJECT
public:
    ClientActionLcd(AbstractServer *parent);
    virtual bool runAction();
    virtual bool setParamsFromMessage(QString message);
private slots:
    void sensorsReady();
private:
    QString display_text;
    bool fixed_mode;
};

#endif // CLIENTACTIONLCD_H
