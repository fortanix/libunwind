# Libunwind customizations for linking with rust-sgx stdlib

## Description
### Intial Fork
* Inital Fork has been made from 5.0 release of libunwind (commit: 29d162446817)
* git diftool 29d162446817     should show the changes done so far.
### Detailed Description
#### Header files that we do not include for this target
1. pthread.h
#### Library that we do not link to for this target.
1. pthread

## Building unwind for rust-sgx target

### Clone the LLVM source tree
* `git clone https://github.com/llvm-mirror/llvm.git`
### Generate Unix Makefiles
* `cd build`
* `cmake -DCMAKE_BUILD_TYPE="RELEASE" -DRUST_SGX=1 -G "Unix Makefiles" -DLLVM_PATH=../../llvm/ ../`
* you could remove `-DCMAKE_BUILD_TYPE="RELEASE"` to enable debug logs of libunwind.
### Build!!
* `make unwind_static`
     build will be found at build/lib/
### Clen build
* cd `build` directory.
* rm -rf ./*
* touch ./.keep
