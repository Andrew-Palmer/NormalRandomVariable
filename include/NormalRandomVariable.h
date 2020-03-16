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

private:
    double mean_;
    double variance_;
};

} // namespace NRV