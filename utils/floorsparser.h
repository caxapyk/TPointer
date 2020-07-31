#ifndef FLOORSTRINGLIST_H
#define FLOORSTRINGLIST_H

#include <QObject>
#include <QStringList>

class FloorsParser : public QObject
{

    Q_OBJECT;

public:
    FloorsParser();
    QStringList process(const QString &floors) const;

};

#endif // FLOORSTRINGLIST_H
