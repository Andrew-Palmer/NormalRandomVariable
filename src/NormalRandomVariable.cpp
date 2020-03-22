#include <stdexcept>

#include "NormalRandomVariable.h"


namespace NRV {

NormalRandomVariable::NormalRandomVariable()
: mean_(0), variance_(1)
{

}

NormalRandomVariable::NormalRandomVariable(double mean, double variance)
: mean_(mean), variance_(variance)
{
    if(variance_ <= 0){
        throw std::range_error("NormalRandomVariable: Variance must be greater than 0");
    }
}

double NormalRandomVariable::mean() const
{
    return mean_;
}

double NormalRandomVariable::variance() const
{
    return variance_;
}

NormalRandomVariable operator+(const NormalRandomVariable& rv1, const NormalRandomVariable& rv2)
{
    return NormalRandomVariable(rv1.mean() + rv2.mean(), rv1.variance() + rv2.variance());
}

NormalRandomVariable operator+(const NormalRandomVariable& rv, double num)
{
    return NormalRandomVariable(rv.mean() + num, rv.variance());
}

NormalRandomVariable operator+(double num, const NormalRandomVariable& rv)
{
    return rv + num;
}

NormalRandomVariable operator-(const NormalRandomVariable& rv1, const NormalRandomVariable& rv2)
{
    return NormalRandomVariable(rv1.mean() - rv2.mean(), rv1.variance() + rv2.variance());
}

NormalRandomVariable operator-(const NormalRandomVariable& rv, double num)
{
    return NormalRandomVariable(rv.mean() - num, rv.variance());
}

NormalRandomVariable operator-(double num, const NormalRandomVariable& rv)
{
    return NormalRandomVariable(num - rv.mean(), rv.variance());
}
    
} // namespace NRV

