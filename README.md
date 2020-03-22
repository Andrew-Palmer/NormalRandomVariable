# NormalRandomVariable
A class implementation of a normal random variable and various operations (under the assumption that they are independent). The operations that are implemented are:

- addition and subtraction (with both normal random variables and constants)

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
