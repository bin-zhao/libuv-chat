rm -rf Build_Server
mkdir Build_Server
cd Build_Server
# export CC=/usr/bin/gcc
# export CXX=/usr/bin/g++
cmake ../Server -DCMAKE_BUILD_TYPE=$1
echo '+++++++++++++++++++++'
make
