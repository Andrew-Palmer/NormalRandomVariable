#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <cmath>
#include <numeric>

#include "NormalRandomVariable.h"

/**
 * Samples from the normal random variables and runs them through function number_of_samples times in order
 * to estimate the resultant distribution
 */
NRV::NormalRandomVariable sampler(double (*function)(std::vector<double>), std::vector<NRV::NormalRandomVariable> inputs, unsigned int number_of_samples)
{
    // Set up the random number generator and store the distributions for each random variable
    std::default_random_engine generator;
    std::vector<std::normal_distribution<double>> distributions;
    for(const auto& rv : inputs)
    {
        distributions.push_back(std::normal_distribution<double>(rv.mean(), std::sqrt(rv.variance())));
    }

    // Sample the function
    std::vector<double> results;
    for(unsigned int i = 0; i < number_of_samples; ++i)
    {
        std::vector<double> inputs_sampled;
        for(auto& distribution : distributions)
        {
            inputs_sampled.push_back(distribution(generator));
        }

        results.push_back(function(inputs_sampled));
    }

    // Calculate the mean and variance of the results
    double mean = std::accumulate(results.begin(), results.end(), 0.0, [=](double a, double b) -> double {return a + b / number_of_samples;});
    double variance = std::accumulate(results.begin(), results.end(), 0.0, [=](double a, double b) -> double {return a + std::pow(b - mean, 2) / number_of_samples;});
    NRV::NormalRandomVariable output(mean, variance);

    return output;
}

TEST(Instantiation, ValidVariance)
{
    EXPECT_NO_THROW(NRV::NormalRandomVariable(1, 1));
}

TEST(Instantiation, InvalidVariance)
{
    EXPECT_ANY_THROW(NRV::NormalRandomVariable(1, 0));
    EXPECT_ANY_THROW(NRV::NormalRandomVariable(1, -1));
}

TEST(Getters, GetMean)
{
    auto rv = NRV::NormalRandomVariable(1,2);
    EXPECT_DOUBLE_EQ(rv.mean(), 1);
}

TEST(Getters, GetVariance)
{
    auto rv = NRV::NormalRandomVariable(1,2);
    EXPECT_DOUBLE_EQ(rv.variance(), 2);
}

TEST(Addition, TwoRandomVariables)
{
    auto rv1 = NRV::NormalRandomVariable(1,2);
    auto rv2 = NRV::NormalRandomVariable(3,4);
    auto rv3 = rv1 + rv2;
    EXPECT_DOUBLE_EQ(rv3.mean(), 4);
    EXPECT_DOUBLE_EQ(rv3.variance(), 6);
}

TEST(Addition, RandomVariableAndConst)
{
    auto rv1 = NRV::NormalRandomVariable(1,2);
    auto rv2 = rv1 + 10;
    EXPECT_DOUBLE_EQ(rv2.mean(), 11);
    EXPECT_DOUBLE_EQ(rv2.variance(), 2);

    rv2 = 12.5 + rv1;
    EXPECT_DOUBLE_EQ(rv2.mean(), 13.5);
    EXPECT_DOUBLE_EQ(rv2.variance(), 2);
}

TEST(Addition, ThreeRandomVariables)
{
    auto rv1 = NRV::NormalRandomVariable(1,2);
    auto rv2 = NRV::NormalRandomVariable(3,4);
    auto rv3 = NRV::NormalRandomVariable(-10,1);
    auto rv4 = rv1 + rv2 + rv3;
    EXPECT_DOUBLE_EQ(rv4.mean(), -6);
    EXPECT_DOUBLE_EQ(rv4.variance(), 7);
}

TEST(Subtraction, TwoRandomVariables)
{
    auto rv1 = NRV::NormalRandomVariable(1,2);
    auto rv2 = NRV::NormalRandomVariable(3,4);
    auto rv3 = rv1 - rv2;
    EXPECT_DOUBLE_EQ(rv3.mean(), -2);
    EXPECT_DOUBLE_EQ(rv3.variance(), 6);
}

TEST(Subtraction, RandomVariableAndConst)
{
    auto rv1 = NRV::NormalRandomVariable(1,2);
    auto rv2 = rv1 - 10;
    EXPECT_DOUBLE_EQ(rv2.mean(), -9);
    EXPECT_DOUBLE_EQ(rv2.variance(), 2);

    rv2 = 12.5 - rv1;
    EXPECT_DOUBLE_EQ(rv2.mean(), 11.5);
    EXPECT_DOUBLE_EQ(rv2.variance(), 2);
}

TEST(Subtraction, ThreeRandomVariables)
{
    auto rv1 = NRV::NormalRandomVariable(1,2);
    auto rv2 = NRV::NormalRandomVariable(3,4);
    auto rv3 = NRV::NormalRandomVariable(-10,1);
    auto rv4 = rv1 - rv2 - rv3;
    EXPECT_DOUBLE_EQ(rv4.mean(), 8);
    EXPECT_DOUBLE_EQ(rv4.variance(), 7);
}

template<class T>
T addition(std::vector<T> inputs)
{
    return inputs[0] + inputs[1];
}

TEST(Sampler, Sampler)
{
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(1, 2));
    inputs.push_back(NRV::NormalRandomVariable(1, 2));
    auto sample_output = sampler(addition<double>, inputs, 1000000);
    auto calc_output = addition<NRV::NormalRandomVariable>(inputs);

    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.01);
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.01);
}

template<class T>
T inverse(std::vector<T> inputs)
{
    return 1 / inputs[0];
}

TEST(Inverse, FarFromZero)
{
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(100, 1));
    auto calc_output = inputs[0].inverse();
    auto sample_output = sampler(inverse<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.01);
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.01);
}

template<class T>
T divideNumByRv(std::vector<T> inputs)
{
    return 5 / inputs[0];
}

TEST(Division, DivideNumByRV)
{
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(100, 1));
    auto calc_output = divideNumByRv<NRV::NormalRandomVariable>(inputs);
    auto sample_output = sampler(divideNumByRv<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.01);
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.01);
}

template<class T>
T divideRvByNum(std::vector<T> inputs)
{
    return inputs[0] / 5;
}

TEST(Division, DivideRVbyNum)
{
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(100, 1));
    auto calc_output = divideRvByNum<NRV::NormalRandomVariable>(inputs);
    auto sample_output = sampler(divideRvByNum<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.01);
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.01);
}

template<class T>
T multiplyRvByNum(std::vector<T> inputs)
{
    return inputs[0] * 0.2;
}

TEST(Multiplication, MultiplyRVbyNum)
{
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(100, 1));
    auto calc_output = multiplyRvByNum<NRV::NormalRandomVariable>(inputs);
    auto sample_output = sampler(multiplyRvByNum<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.01);
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.01);
}

template<class T>
T multiplyNumByRv(std::vector<T> inputs)
{
    return 0.2 * inputs[0];
}

TEST(Multiplication, MultiplyNumbyRV)
{
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(100, 1));
    auto calc_output = multiplyNumByRv<NRV::NormalRandomVariable>(inputs);
    auto sample_output = sampler(multiplyNumByRv<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.01);
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.01);
}