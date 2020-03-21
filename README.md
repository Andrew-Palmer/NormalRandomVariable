# NormalRandomVariable
A class implementation of a normal random variable and various operations


## Testing requirements

Install Google gtest

'''
sudo apt update
sudo apt install libgtest-dev
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp *.a /usr/lib
'''

## Build & Test

'''
mkdir build
cd build
cmake ..
make
./test/nrv_test
'''