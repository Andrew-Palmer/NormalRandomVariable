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
    // This approximation is breaks down if the distribution is too close to 0. Set an arbitrary limit of 4 sigma. 
    if(mean_ * mean_ / variance_ < 16)
    {
        throw std::range_error("NormalRandomVariable: Variance of denominator is too large to allow approximation of division operator");
    }

    double mean_squared = mean_ * mean_;
    double mean = mean_ / (mean_squared - variance_);
    double variance = variance_ / (mean_squared * mean_squared - 2 * mean_squared * variance_ + variance_ * variance_);

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
    double a = rv1.mean() * rv1.mean() / rv1.variance();
    double b = rv2.mean() * rv2.mean() / rv2.variance();

    // Check that the conditions for the approximation are met
    if(a < 6.25 && b >= 16)
    {
        double r = rv2.variance() / rv1.variance();
        double sqrt_b = std::sqrt(b);
        double mean = std::sqrt(a) / (std::sqrt(r) * (1.01 * sqrt_b - 0.2713));
        double variance = (a + 1) / (r * (b + 0.108 * sqrt_b - 3.795)) - mean * mean;

        return NormalRandomVariable(mean, variance);
    }
    else
    {
        // Otherwise, approximate it by multiplying rv1 by the inverse of rv2
        return rv1 * rv2.inverse();
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
    double delta1 = rv1.mean() * rv1.mean() / rv1.variance();
    double delta2 = rv2.mean() * rv2.mean() / rv2.variance();
    double variance = rv1.variance() * rv2.variance() * (1 + delta1 + delta2);

    return NormalRandomVariable(mean, variance);
}
    
} // namespace NRV

