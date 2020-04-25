#include <iostream>

#include "NormalRandomVariable.h"

int main(int argc, char *argv[])
{
    NRV::NormalRandomVariable var(1,4);

    std::cout << "Created a normally distributed random variable with mean of " << var.mean() << " and variance of " << var.variance() <<std::endl;
}