# NormalRandomVariable
A class implementation of a normal random variable and various operations (under the assumption that they are independent). The operations that are implemented are:

- addition and subtraction (with both normal random variables and constants)
- inverse
    - Note: this operation will throw an exception if the mean of the random variable is less than 4 times the standard deviation
- multiplication and division (with both normal random variables and constants)
    - Note: division of 2 random variables will throw an exception if the mean of the denominator is less than 4 times its standard deviation
- rectification using arbitrary lower and upper bounds
    - Rectifying a distribution groups the probability mass outside of the bounds at the respective bound
- truncation using arbitrary lower and upper bounds (where the bounds are either scalars or normally distributed random variables)
    - Truncating a distribution discards all probability mass outside the bounds
- maximum and minimum of two random variables

Note: This implementation does not model covariance, and it is up to the user to ensure that the random variables, and equations of random variables, are independent. 

## Building

    mkdir build
    cd build
    cmake ..
    make

## Testing

A number of tests have been implemented that use Monte Carlo simulation to verify that the approximations are performing correctly. 

Install Google gtest

    sudo apt update
    sudo apt install libgtest-dev
    cd /usr/src/gtest
    sudo cmake CMakeLists.txt
    sudo make
    sudo cp *.a /usr/lib

Enable testing when building and run the test

    mkdir build
    cd build
    cmake -DBUILD_TESTS=ON ..
    make
    make test ARGS="-V"

## Installation

The library can be installed after it has been built using:

    sudo make install

## Example

A usage example is provided in the `example` folder. 

## References

The approximations used for the various operations are presented in the following papers:

- Palmer, A. W., Hill, A. J., & Scheding, S. J. (2017). Methods for Stochastic Collection and Replenishment (SCAR) optimisation for persistent autonomy. _Robotics and Autonomous Systems_, 87, 51-65.
- Palmer, A. W., Hill, A. J., & Scheding, S. J. (2018, May). Modelling resource contention in multi-robot task allocation problems with uncertain timing. In _2018 IEEE International Conference on Robotics and Automation (ICRA)_ (pp. 1-8). IEEE.
- Simon, D., & Simon, D. L. (2010). Constrained Kalman filtering via density function truncation for turbofan engine health estimation. _International Journal of Systems Science, 41(2)_, 159-171.
