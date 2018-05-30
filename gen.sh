rm -rf eosdacvote.abi
rm -rf eosdacvote.wast
eosiocpp -g eosdacvote.abi eosdacvote.cpp
eosiocpp -o eosdacvote.wast eosdacvote.cpp
