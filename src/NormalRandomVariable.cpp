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




    
} // namespace NRV

