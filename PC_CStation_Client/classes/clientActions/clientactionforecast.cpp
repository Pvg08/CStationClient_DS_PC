#include "clientactionforecast.h"

ClientActionForecast::ClientActionForecast(AbstractServer *parent):
    ClientAction(parent)
{
    param_prefix = "SET_FORECAST";
    item_name = tr("Forecast");
    description_string = "{'CODE':'setforecast','PREFIX':'SET_FORECAST','PARAM':[{'NAME':'Forecast','TYPE':'STRING'}],'BUTTONS':[{'NAME':'Request','PARAMSET':['R']}]}";
}

bool ClientActionForecast::runAction()
{
    emit updateState();
}

bool ClientActionForecast::setParamsFromMessage(QString message)
{
    settings->insert("TEXT", message);
    return true;
}

