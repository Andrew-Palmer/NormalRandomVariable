#pragma once

namespace NRV {

/**
 * Class that implements an independent normal random variable and various operations
 */
class NormalRandomVariable {
public:
    /**
     * Constructor for a NRV with a standard normal distribution
     */
    NormalRandomVariable();

    /**
     * Constructor for a NRV with specified mean and variance
     * Note: Will throw an exception if variance is not greater than 0
     */
    NormalRandomVariable(double mean, double variance);

    /**
     * Get the mean of the random variable
     */
    double mean() const;

    /**
     * Get the variance of the random variable
     */
    double variance() const;

    /**
     * Calculates the inverse of the random variable (i.e., 1/x where x is the random variable)
     */
    NormalRandomVariable inverse() const;

private:
    double mean_;
    double variance_;
};

/**
 * Addition of 2 random variables, or 1 random variable with a constant
 */
NormalRandomVariable operator+(const NormalRandomVariable& rv1, const NormalRandomVariable& rv2);
NormalRandomVariable operator+(const NormalRandomVariable& rv, double num);
NormalRandomVariable operator+(double num, const NormalRandomVariable& rv);

/**
 * Subtraction of 2 random variables, or 1 random variable with a constant
 */
NormalRandomVariable operator-(const NormalRandomVariable& rv1, const NormalRandomVariable& rv2);
NormalRandomVariable operator-(const NormalRandomVariable& rv, double num);
NormalRandomVariable operator-(double num, const NormalRandomVariable& rv);

/**
 * Division of random variable with a constant
 */
NormalRandomVariable operator/(const NormalRandomVariable& rv, double num);
NormalRandomVariable operator/(double num, const NormalRandomVariable& rv);

} // namespace NRV