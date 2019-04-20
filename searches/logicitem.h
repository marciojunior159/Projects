#ifndef LOGICITEM_H
#define LOGICITEM_H

#include <QTableWidget>

class LogicItem : public QTableWidgetItem
{
    Q_OBJECT
private:
    int distance;

public:
    explicit LogicItem(QObject *parent = 0);
    int getDistance();
    void setDistance(int);

};

#endif // LOGICITEM_H
