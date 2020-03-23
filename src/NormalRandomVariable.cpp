#include <stdexcept>
#include <cmath>

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

NormalRandomVariable NormalRandomVariable::inverse() const
{
    double mean = mean_ / (std::pow(mean_, 2) - variance_);
    double variance = variance_ / (std::pow(mean_, 4) - 2 * std::pow(mean_, 2) * variance_ + std::pow(variance_, 2));

    return NormalRandomVariable(mean, variance);
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

NormalRandomVariable operator/(const NormalRandomVariable& rv, double num)
{
    return NormalRandomVariable(rv.mean() / num, rv.variance() / std::pow(num, 2));
}

NormalRandomVariable operator/(double num, const NormalRandomVariable& rv)
{
    auto inverse = rv.inverse();
    return NormalRandomVariable(inverse.mean() * num, inverse.variance() * std::pow(num, 2));
}

NormalRandomVariable operator/(const NormalRandomVariable& rv1, const NormalRandomVariable& rv2)
{
    double r = rv2.variance() / rv1.variance();
    double a = rv1.mean() / std::sqrt(rv1.variance());
    double b = rv2.mean() / std::sqrt(rv2.variance());

    // Check that the conditions for the approximation are met
    if(a < 2.5 && b > 4)
    {
        double mean = a / (std::sqrt(r) * (1.01 * b - 0.2713));
        double variance = (std::pow(a, 2) + 1) / (r * (std::pow(b, 2) + 0.108 * b - 3.795)) - std::pow(mean, 2);

        return NormalRandomVariable(mean, variance);
    }
    else
    {
        // Otherwise, approximate it dividing the mean of rv1 by rv2
        return rv1.mean() / rv2;
    }
}

NormalRandomVariable operator*(const NormalRandomVariable& rv, double num)
{
    return NormalRandomVariable(rv.mean() * num, rv.variance() * std::pow(num, 2));
}

NormalRandomVariable operator*(double num, const NormalRandomVariable& rv)
{
    return NormalRandomVariable(rv.mean() * num, rv.variance() * std::pow(num, 2));
}

NormalRandomVariable operator*(const NormalRandomVariable& rv1, const NormalRandomVariable& rv2)
{
    double mean = rv1.mean() * rv2.mean();
    double delta1 = std::pow(rv1.mean(), 2) / rv1.variance();
    double delta2 = std::pow(rv2.mean(), 2) / rv2.variance();
    double variance = rv1.variance() * rv2.variance() * (1 + delta1 + delta2);

    return NormalRandomVariable(mean, variance);
}
    
} // namespace NRV

