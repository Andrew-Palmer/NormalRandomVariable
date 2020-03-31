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

        // Use a try-catch block to handle cases where the function should not actually return a result
        try
        {
            results.push_back(function(inputs_sampled));
        }
        catch (...)
        {
            // Do nothing
        }
    }

    // Calculate the mean and variance of the results
    double mean = std::accumulate(results.begin(), results.end(), 0.0, [=](double a, double b) -> double {return a + b / results.size();});
    double variance = std::accumulate(results.begin(), results.end(), 0.0, [=](double a, double b) -> double {return a + std::pow(b - mean, 2) / results.size();});
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

TEST(Negation, RandomVariable)
{
    auto rv1 = NRV::NormalRandomVariable(1,2);
    auto rv2 = -rv1;
    EXPECT_DOUBLE_EQ(rv2.mean(), -1);
    EXPECT_DOUBLE_EQ(rv2.variance(), 2);
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

TEST(Inverse, CloseToZero)
{
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(1, 1));
    EXPECT_ANY_THROW(inputs[0].inverse());
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
T divideRvByRv(std::vector<T> inputs)
{
    return inputs[0] /inputs[1];
}

TEST(Division, DivideRVbyRV)
{
    // First check with distributions that fulfill the approximation requirements
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(10, 25));
    inputs.push_back(NRV::NormalRandomVariable(5, 1));
    auto calc_output = divideRvByRv<NRV::NormalRandomVariable>(inputs);
    auto sample_output = sampler(divideRvByRv<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.01);
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.1); // Higher threshold for variance

    // Then check what happens if the numerator has a lower variance
    inputs[0] = NRV::NormalRandomVariable(10, 1);
    calc_output = divideRvByRv<NRV::NormalRandomVariable>(inputs);
    sample_output = sampler(divideRvByRv<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.01);
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.1); // Higher threshold for variance

     // Then check what happens if the denominator has too high variance
     // Note: this would result in a multi-modal distribution and cannot be handled
    inputs[0] = NRV::NormalRandomVariable(10, 25);
    inputs[1] = NRV::NormalRandomVariable(5, 2);
    EXPECT_ANY_THROW(divideRvByRv<NRV::NormalRandomVariable>(inputs));
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

template<class T>
T multiplyRvByRv(std::vector<T> inputs)
{
    return inputs[0] * inputs[1];
}

TEST(Multiplication, MultiplyRVbyRV)
{
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(10, 0.5));
    inputs.push_back(NRV::NormalRandomVariable(20, 0.2));
    auto calc_output = multiplyRvByRv<NRV::NormalRandomVariable>(inputs);
    auto sample_output = sampler(multiplyRvByRv<double>, inputs, 1000000);
    
    // Note: needed to relax the threshold for saying that they are near because the
    // multiplication magnifies sampling error
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.1); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.1); 
}

template<class T>
T rectify(std::vector<T> inputs)
{
    // Use a lower bound of 0 and an upper bound of 10
    if(inputs[0] < 0)
    {
        return 0;
    }
    if(inputs[0] > 10)
    {
        return 10;
    }

    return inputs[0];
}

TEST(Rectification, RectificationWithUpperAndLower)
{
    // Try close to the upper bound
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(10, 0.5));
    auto calc_output = inputs[0].rectify(0, 10);
    auto sample_output = sampler(rectify<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 

    // Try close to the lower bound
    inputs[0] = NRV::NormalRandomVariable(0, 0.5);
    calc_output = inputs[0].rectify(0, 10);
    sample_output = sampler(rectify<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 

    // Try close to both the upper and lower bounds
    inputs[0] = NRV::NormalRandomVariable(5, 10);
    calc_output = inputs[0].rectify(0, 10);
    sample_output = sampler(rectify<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 
}

template<class T>
T rectifyLower(std::vector<T> inputs)
{
    // Use a lower bound of 0
    if(inputs[0] < 0)
    {
        return 0;
    }

    return inputs[0];
}

TEST(Rectification, RectificationWithLower)
{
    // Try far from the lower bound
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(10, 0.5));
    auto calc_output = inputs[0].rectifyLower(0);
    auto sample_output = sampler(rectifyLower<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 

    // Try close to the lower bound
    inputs[0] = NRV::NormalRandomVariable(0, 0.5);
    calc_output = inputs[0].rectifyLower(0);
    sample_output = sampler(rectifyLower<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 
}

template<class T>
T rectifyUpper(std::vector<T> inputs)
{
    // Use a lower bound of 0
    if(inputs[0] > 10)
    {
        return 10;
    }

    return inputs[0];
}

TEST(Rectification, RectificationWithUpper)
{
    // Try at the upper bound
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(10, 0.5));
    auto calc_output = inputs[0].rectifyUpper(10);
    auto sample_output = sampler(rectifyUpper<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 

    // Try far from the upper bound
    inputs[0] = NRV::NormalRandomVariable(0, 0.5);
    calc_output = inputs[0].rectifyUpper(10);
    sample_output = sampler(rectifyUpper<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 
}

template<class T>
T truncate(std::vector<T> inputs)
{
    // Use a lower bound of 0 and an upper bound of 10
    if(inputs[0] < 0)
    {
        throw "Outside of bounds";
    }
    if(inputs[0] > 10)
    {
        throw "Outside of bounds";
    }

    return inputs[0];
}

TEST(Truncation, TruncationWithUpperAndLower)
{
    // Try close to the upper bound
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(10, 0.5));
    auto calc_output = inputs[0].truncate(0, 10);
    auto sample_output = sampler(truncate<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 

    // Try close to the lower bound
    inputs[0] = NRV::NormalRandomVariable(0, 0.5);
    calc_output = inputs[0].truncate(0, 10);
    sample_output = sampler(truncate<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 

    // Try close to both the upper and lower bounds
    inputs[0] = NRV::NormalRandomVariable(5, 10);
    calc_output = inputs[0].truncate(0, 10);
    sample_output = sampler(truncate<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 

    // Try upper and lower bounds that are both higher than the mean
    inputs[0] = NRV::NormalRandomVariable(-2, 10);
    calc_output = inputs[0].truncate(0, 10);
    sample_output = sampler(truncate<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 
}

TEST(Truncation, TruncationWithInvalidBounds)
{
    // Incorrect bounds should throw an exception
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(10, 0.5));
    EXPECT_ANY_THROW(inputs[0].truncate(10, 5));
    EXPECT_ANY_THROW(inputs[0].truncate(10, 10));
}

template<class T>
T truncateLower(std::vector<T> inputs)
{
    // Use a lower bound of 0 and an upper bound of 10
    if(inputs[0] < 0)
    {
        throw "Outside of bounds";
    }

    return inputs[0];
}

TEST(Truncation, TruncationWithLower)
{
    // Try far from the bound
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(10, 0.5));
    auto calc_output = inputs[0].truncateLower(0);
    auto sample_output = sampler(truncateLower<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 

    // Try equal to the bound
    inputs[0] = NRV::NormalRandomVariable(0, 0.5);
    calc_output = inputs[0].truncateLower(0);
    sample_output = sampler(truncateLower<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 

    // Try below the lower bound
    inputs[0] = NRV::NormalRandomVariable(-2, 1);
    calc_output = inputs[0].truncateLower(0);
    sample_output = sampler(truncateLower<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 
}


template<class T>
T truncateUpper(std::vector<T> inputs)
{
    // Use a lower bound of 0 and an upper bound of 10
    if(inputs[0] > 10)
    {
        throw "Outside of bounds";
    }

    return inputs[0];
}

TEST(Truncation, TruncationWithUpper)
{
    // Try equal to the bound
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(10, 0.5));
    auto calc_output = inputs[0].truncateUpper(10);
    auto sample_output = sampler(truncateUpper<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 

    // Try far from the bound
    inputs[0] = NRV::NormalRandomVariable(0, 0.5);
    calc_output = inputs[0].truncateUpper(10);
    sample_output = sampler(truncateUpper<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 

    // Try above the upper bound
    inputs[0] = NRV::NormalRandomVariable(12, 1);
    calc_output = inputs[0].truncateUpper(10);
    sample_output = sampler(truncateUpper<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 
}


template<class T>
T truncateSoftBounds(std::vector<T> inputs)
{
    if(inputs[0] < inputs[1])
    {
        throw "Outside of bounds";
    }
    if(inputs[0] > inputs[2])
    {
        throw "Outside of bounds";
    }

    return inputs[0];
}

TEST(Truncation, TruncationWithUpperAndLowerSoftBounds)
{
    // Try close to the upper bound
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(10, 0.5));
    NRV::NormalRandomVariable lower(0, 1);
    NRV::NormalRandomVariable upper(10, 1);
    inputs.push_back(lower);
    inputs.push_back(upper);
    auto calc_output = inputs[0].truncate(lower, upper);
    auto sample_output = sampler(truncateSoftBounds<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 

    // Try close to the lower bound
    inputs[0] = NRV::NormalRandomVariable(0, 0.5);
    calc_output = inputs[0].truncate(lower, upper);
    sample_output = sampler(truncateSoftBounds<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 

    // Try close to both the upper and lower bounds
    inputs[0] = NRV::NormalRandomVariable(5, 10);
    calc_output = inputs[0].truncate(lower, upper);
    sample_output = sampler(truncateSoftBounds<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 

    // Try with a lower bound that is higher than the upper bound
    inputs[0] = NRV::NormalRandomVariable(5, 1);
    lower = NRV::NormalRandomVariable(6, 2);
    upper = NRV::NormalRandomVariable(4, 2);
    inputs[1] = lower;
    inputs[2] = upper;

    calc_output = inputs[0].truncate(lower, upper);
    sample_output = sampler(truncateSoftBounds<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 
}

template<class T>
T truncateSoftBoundsLower(std::vector<T> inputs)
{
    if(inputs[0] < inputs[1])
    {
        throw "Outside of bounds";
    }

    return inputs[0];
}

TEST(Truncation, TruncationWithLowerSoftBounds)
{
    // Try far from the lower bound
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(10, 0.5));
    NRV::NormalRandomVariable lower(0, 1);
    inputs.push_back(lower);
    auto calc_output = inputs[0].truncateLower(lower);
    auto sample_output = sampler(truncateSoftBoundsLower<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 

    // Try at the lower bound
    inputs[0] = NRV::NormalRandomVariable(0, 0.5);
    calc_output = inputs[0].truncateLower(lower);
    sample_output = sampler(truncateSoftBoundsLower<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 

    // Try less than the lower bound
    inputs[0] = NRV::NormalRandomVariable(-2, 1);
    calc_output = inputs[0].truncateLower(lower);
    sample_output = sampler(truncateSoftBoundsLower<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 
}


template<class T>
T truncateSoftBoundsUpper(std::vector<T> inputs)
{
    if(inputs[0] > inputs[1])
    {
        throw "Outside of bounds";
    }

    return inputs[0];
}

TEST(Truncation, TruncationWithUpperSoftBounds)
{
    // Try at the upper bound
    std::vector<NRV::NormalRandomVariable> inputs;
    inputs.push_back(NRV::NormalRandomVariable(10, 0.5));
    NRV::NormalRandomVariable upper(10, 1);
    inputs.push_back(upper);
    auto calc_output = inputs[0].truncateUpper(upper);
    auto sample_output = sampler(truncateSoftBoundsUpper<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 

    // Try far from the lower bound
    inputs[0] = NRV::NormalRandomVariable(0, 0.5);
    calc_output = inputs[0].truncateUpper(upper);
    sample_output = sampler(truncateSoftBoundsUpper<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 

    // Try higher than the upper bound
    inputs[0] = NRV::NormalRandomVariable(12, 1);

    calc_output = inputs[0].truncateUpper(upper);
    sample_output = sampler(truncateSoftBoundsUpper<double>, inputs, 1000000);
    
    EXPECT_NEAR(calc_output.mean(), sample_output.mean(), 0.02); 
    EXPECT_NEAR(calc_output.variance(), sample_output.variance(), 0.02); 
}
