#include "signalfunction.h"

#include <QPointF>
#include <QtMath>

SignalFunction::SignalFunction() : a(10.0), f(80.0)
{
}

SignalFunction::SignalFunction(double a, double f) : a(a), f(f)
{
}

QPointF SignalFunction::function(double ts) const
{
    return QPointF(a * qCos(2 * M_PI * f * ts), a * qSin(2 * M_PI * f * ts));
}

void SignalFunction::setA(double newA)
{
    a = newA;
}

void SignalFunction::setF(double newF)
{
    f = newF;
}
