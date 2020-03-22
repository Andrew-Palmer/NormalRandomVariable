# NormalRandomVariable
A class implementation of a normal random variable and various operations (under the assumption that they are independent). The operations that are implemented are:

- addition and subtraction (with both normal random variables and constants)
- inverse

Note: This implementation does not model covariance, and it is up to the user to ensure that the random variables, and equations of random variables, are independent. 

## Dependencies

Install Google gtest

    sudo apt update
    sudo apt install libgtest-dev
    cd /usr/src/gtest
    sudo cmake CMakeLists.txt
    sudo make
    sudo cp *.a /usr/lib


## Build & Test

    mkdir build
    cd build
    cmake ..
    make
    ./test/nrv_test

## References

The approximations used for the various operations are presented in the following papers:

- Palmer, A. W., Hill, A. J., & Scheding, S. J. (2017). Methods for Stochastic Collection and Replenishment (SCAR) optimisation for persistent autonomy. _Robotics and Autonomous Systems_, 87, 51-65.

