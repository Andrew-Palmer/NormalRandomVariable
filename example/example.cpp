#include <iostream>

#include "NormalRandomVariable.h"

int main(int argc, char *argv[])
{
    NRV::NormalRandomVariable a(100,16);
    NRV::NormalRandomVariable b(110,36);

    std::cout << "Two cars are approaching an intersection." << std::endl;
    std::cout << "The arrival time of car A is described by a normally distributed random variable with mean " << a.mean() << " seconds and variance " << a.variance() << "." << std::endl;
    std::cout << "The arrival time of car B is described by a normally distributed random variable with mean " << b.mean() << " seconds and variance " << b.variance() << "." << std::endl;
    
    auto min_time = a.min(b);
    std::cout << "Given that either car could arrive first, the first car is expected to arrive with a time distribution with mean " << min_time.mean() << " seconds and variance " << min_time.variance() << "." << std::endl;

    auto max_time = a.max(b);
    std::cout << "The second car is expected to arrive with a time distribution with mean " << max_time.mean() << " seconds and variance " << max_time.variance() << "." << std::endl;

    auto a_given_a_arrives_first = a.truncateUpper(b);
    std::cout << "The conditional distribution of the arrival time of car A if it arrives before car B has mean " << a_given_a_arrives_first.mean() << " seconds and variance " << a_given_a_arrives_first.variance() << "." << std::endl;

    auto a_given_b_arrives_first = a.truncateLower(b);
    std::cout << "The conditional distribution of the arrival time of car A if it arrives after car B has mean " << a_given_b_arrives_first.mean() << " seconds and variance " << a_given_b_arrives_first.variance() << "." << std::endl;
}