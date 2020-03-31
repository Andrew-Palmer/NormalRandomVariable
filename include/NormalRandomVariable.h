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

    /**
     * Returns a rectified normal variable between lower and upper bounds
     */
    NormalRandomVariable rectify(double lower, double upper) const;
    
    /**
     * Returns a rectified normal variable, rectified using a lower bound of 0
     */
    NormalRandomVariable rectify() const;

    /**
     * Returns a truncated normal variable between the lower and upper bounds
     */
    NormalRandomVariable truncate(double lower, double upper) const;

    /**
     * Returns a truncated normal variable above the lower bound
     */
    NormalRandomVariable truncateLower(double lower) const;

    /**
     * Returns a truncated normal variable under the upper bound
     */
    NormalRandomVariable truncateUpper(double upper) const;

    /**
     * Returns a truncated normal variable between the lower and upper bounds
     */
    NormalRandomVariable truncate(NormalRandomVariable lower, NormalRandomVariable upper) const;

    /**
     * Returns a truncated normal variable above the lower bound
     */
    NormalRandomVariable truncateLower(NormalRandomVariable lower) const;

    /**
     * Returns a truncated normal variable under the upper bound
     */
    NormalRandomVariable truncateUpper(NormalRandomVariable upper) const;


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
 * Negation
 */
NormalRandomVariable operator-(const NormalRandomVariable& rv);

/**
 * Division of random variable with a constant
 */
NormalRandomVariable operator/(const NormalRandomVariable& rv, double num);
NormalRandomVariable operator/(double num, const NormalRandomVariable& rv);

/**
 * Division of 2 random variables
 * Note: The approximation used has very strict rules on when it is valid. Outside of these
 * rules (i.e., when the variance of rv1 becomes too small), the distribution approaches
 * simply division of the mean of rv1 by the random variable rv2. 
 */
NormalRandomVariable operator/(const NormalRandomVariable& rv1, const NormalRandomVariable& rv2);

/**
 * Multiplication of random variable with a constant
 */
NormalRandomVariable operator*(const NormalRandomVariable& rv, double num);
NormalRandomVariable operator*(double num, const NormalRandomVariable& rv);

/**
 * Multiplication of 2 random variables
 */
NormalRandomVariable operator*(const NormalRandomVariable& rv1, const NormalRandomVariable& rv2);

} // namespace NRV