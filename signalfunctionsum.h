#ifndef SIGNALFUNCTIONSUM_H
#define SIGNALFUNCTIONSUM_H

#include "signalfunction.h"

class SignalFunctionSum : public SignalFunction
{
public:
    SignalFunctionSum();
    virtual QPointF function(double ts) const override;

    void setA2(double newA2);

    void setF2(double newF2);

private:
    double a2, f2;
};

#endif // SIGNALFUNCTIONSUM_H
