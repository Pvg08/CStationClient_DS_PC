#include "clientsensorbtnactivity.h"

ClientSensorBtnActivity::ClientSensorBtnActivity(AbstractServer *parent):
    ClientSensor(parent)
{
    sensor_code = "O";
    item_name = tr("Action button");
    description_string = "{'CODE':'"+sensor_code+"','NAME':'Outer signal','TYPE':'ENUM','ENUMS':['no','yes']}";
}

QString ClientSensorBtnActivity::getValueString()
{
    return "'" + sensor_code + "':'"+settings->value("VALUE","no")+"'";
}
