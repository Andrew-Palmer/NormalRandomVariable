#pragma once

namespace NRV {

/**
 * 
 */
class NormalRandomVariable {
public:
    /*
     * 
     */
    NormalRandomVariable(double mean, double variance);

private:
    double mean_;
    double variance_;
};

} // namespace NRV