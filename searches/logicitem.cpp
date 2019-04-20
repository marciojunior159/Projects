#include "logicitem.h"

LogicItem::LogicItem(QObject *parent)
    : QTableWidgetItem(parent),
      ui(new ui::QTableWidgetItem)
{

}

int LogicItem::getDistance()
{
    return distance;
}

void LogicItem::setDistance(int d)
{
    distance = d;
}

