#include "clientsensoractivity.h"

ClientSensorActivity::ClientSensorActivity(AbstractServer *parent):
    ClientSensor(parent)
{
    sensor_code = "A";
    item_name = tr("Client activity");
    description_string = "{'CODE':'"+sensor_code+"','NAME':'Activity','TIMEOUT':60,'TYPE':'ENUM','ENUMS':['off','on']}";
}

QString ClientSensorActivity::getValueString()
{
    return "'" + sensor_code + "':'on'";
}
