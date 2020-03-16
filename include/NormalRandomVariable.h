#pragma once

namespace NRV {

/**
 * Class that implements an independent normal random variable
 */
class NormalRandomVariable {
public:
    /**
     * Constructors
     */
    NormalRandomVariable();
    NormalRandomVariable(double mean, double variance);

private:
    double mean_;
    double variance_;
};

} // namespace NRV