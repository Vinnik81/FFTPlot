#ifndef SIGNALFUNCTION_H
#define SIGNALFUNCTION_H



class QPointF;
class SignalFunction
{
public:
    SignalFunction();
    SignalFunction(double a, double f);
    virtual QPointF function(double ts) const;

    void setA(double newA);

    void setF(double newF);

protected:
    double a;
    double f;
};

#endif // SIGNALFUNCTION_H
