#include "signalfunctionsum.h"

#include <QPointF>
#include <QtMath>

SignalFunctionSum::SignalFunctionSum(): a2(30), f2(180)
{

}

QPointF SignalFunctionSum::function(double ts) const
{
    return QPointF(a * qCos(2 * M_PI * f * ts) + a2 * qCos(2 * M_PI * f2 * ts), a * qSin(2 * M_PI * f * ts) + a2 * qSin(2 * M_PI * f2 * ts));
}

void SignalFunctionSum::setA2(double newA2)
{
    a2 = newA2;
}

void SignalFunctionSum::setF2(double newF2)
{
    f2 = newF2;
}
