rm -rf edtmarket.abi
rm -rf edtmarket.wast
eosiocpp -g edtmarket.abi edtmarket.cpp
eosiocpp -o edtmarket.wast edtmarket.cpp
