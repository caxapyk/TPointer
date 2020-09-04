#include "floorsparser.h"

FloorsParser::FloorsParser() : QObject()
{

}

QStringList FloorsParser::process(const QString &floors) const{
    QStringList fList = floors.split(",");

     /* replace 0 floor with 'basement' */
    fList = fList.replaceInStrings(QRegExp("^[0]+$"), tr("bsmnt"));

    return fList;
}
