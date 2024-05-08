#!/bin/bash
set -e
set -x

# Set directory
SCRIPTPATH=`realpath .`
export ANDROID_NDK_HOME=/home/simone/Android/Sdk/ndk/17.2.4988734
OPENSSL_DIR=$SCRIPTPATH/openssl-1.1.1k

# Comment/Uncomment for armeabi
PATH=$ANDROID_NDK_HOME/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin:$PATH
# Comment/Uncomment for x86
#PATH=$ANDROID_NDK_HOME/toolchains/x86-4.9/prebuilt/linux-x86_64/bin:$PATH

# The target architecture can be 
# android-arm, android-arm64, android-x86, android-x86 etc
# Comment/Uncomment for armeabi
architecture=android-arm
# Comment/Uncomment for x86
#architecture=android-x86
cd ${OPENSSL_DIR}
./Configure ${architecture} -D__ANDROID_API__=15
make clean
make

# Dare make clean quando si cambia architettura

# Copy the outputs
OUTPUT_INCLUDE=$SCRIPTPATH/output/include
OUTPUT_LIB=$SCRIPTPATH/output/lib/${architecture}
mkdir -p $OUTPUT_INCLUDE
mkdir -p $OUTPUT_LIB
cp -RL include/openssl $OUTPUT_INCLUDE
cp libcrypto.so $OUTPUT_LIB
cp libcrypto.a $OUTPUT_LIB
cp libssl.so $OUTPUT_LIB
cp libssl.a $OUTPUT_LIB

