# Copyright 2020 Rene Rivera
# Copyright 2022 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

globalenv={ 'UBSAN_OPTIONS': 'print_stacktrace=1' }

def main(ctx):
  return [
  linux_cxx("GCC 4.4 14.04", "", packages="g++-4.4", buildscript="drone", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-4.4', 'CXXSTD': '98,0x'}, globalenv=globalenv),
  linux_cxx("GCC 4.6 14.04", "", packages="g++-4.6", buildscript="drone", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-4.6', 'CXXSTD': '03,0x'}, globalenv=globalenv),
  linux_cxx("GCC 4.8 14.04", "", packages="g++-multilib", buildscript="drone", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++', 'CXXSTD': '03,11', 'ADDRMD': '32,64'}, globalenv=globalenv),
  linux_cxx("GCC 4.9 14.04", "", packages="g++-4.9", buildscript="drone", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-4.9', 'CXXSTD': '03,11'}, globalenv=globalenv),
  linux_cxx("GCC 5 16.04", "", packages="g++-multilib", buildscript="drone", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++', 'CXXSTD': '03,11,14', 'ADDRMD': '32,64'}, globalenv=globalenv),
  linux_cxx("GCC 6 16.04", "", packages="g++-6", buildscript="drone", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-6', 'CXXSTD': '03,11'}, globalenv=globalenv),
  linux_cxx("GCC 7 18.04", "", packages="g++-multilib", buildscript="drone", image="cppalliance/droneubuntu1804:1", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++', 'CXXSTD': '03,11,14,1z', 'ADDRMD': '32,64'}, globalenv=globalenv),
  linux_cxx("GCC 8 18.04", "", packages="g++-8", buildscript="drone", image="cppalliance/droneubuntu1804:1", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-8', 'CXXSTD': '03,11,14,1z', 'ADDRMD': '32,64'}, globalenv=globalenv),
  linux_cxx("GCC 9 20.04", "", packages="g++-multilib", buildscript="drone", image="cppalliance/droneubuntu2004:1", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++', 'CXXSTD': '03,11,14,17,2a', 'ADDRMD': '32,64'}, globalenv=globalenv),
  linux_cxx("GCC 9 20.04 ARM64", "", packages="", buildscript="drone", image="cppalliance/droneubuntu2004:multiarch", arch="arm64", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++', 'CXXSTD': '03,11,14,17,2a', 'ADDRMD': '32,64'}, globalenv=globalenv),
  linux_cxx("GCC 10 20.04 UBSAN", "", packages="g++-10", buildscript="drone", image="cppalliance/droneubuntu2004:1", environment={'UBSAN': '1', 'TOOLSET': 'gcc', 'COMPILER': 'g++-10', 'CXXSTD': '03,11,14,17,20', 'LINKFLAGS': '-fuse-ld=gold'}, globalenv=globalenv),
  linux_cxx("Clang 3.3", "", packages="clang-3.3", buildscript="drone", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'clang', 'COMPILER': '/usr/bin/clang++', 'CXXSTD': '03,11'}, globalenv=globalenv),
  linux_cxx("Clang 3.4", "", packages="clang-3.4", buildscript="drone", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'clang', 'COMPILER': '/usr/bin/clang++', 'CXXSTD': '03,11'}, globalenv=globalenv),
  linux_cxx("Clang 13 UBSAN", "", packages="clang-13", llvm_os="focal", llvm_ver="13", buildscript="drone", image="cppalliance/droneubuntu2004:1", environment={'UBSAN': '1', 'TOOLSET': 'clang', 'COMPILER': 'clang++-13', 'CXXSTD': '03,11,14,17,20'}, globalenv=globalenv),
  linux_cxx("Clang 14 UBSAN", "", packages="clang-14", llvm_os="focal", llvm_ver="14", buildscript="drone", image="cppalliance/droneubuntu2004:1", environment={'UBSAN': '1', 'TOOLSET': 'clang', 'COMPILER': 'clang++-14', 'CXXSTD': '03,11,14,17,20'}, globalenv=globalenv),
  osx_cxx("Clang UBSAN", "", packages="", buildtype="boost", buildscript="drone", xcode_version="12.2", environment={'UBSAN': '1', 'TOOLSET': 'clang', 'COMPILER': 'clang++', 'CXXSTD': '03,11,14,1z'}, globalenv=globalenv),
  windows_cxx("msvc-14.0", "", image="cppalliance/dronevs2015", buildscript="drone", environment={"TOOLSET": "msvc-14.0", "CXXSTD": "14,latest"}, globalenv=globalenv),
  windows_cxx("msvc-14.1", "", image="cppalliance/dronevs2017", buildscript="drone", environment={"TOOLSET": "msvc-14.1", "CXXSTD": "14,17,latest"}, globalenv=globalenv),
  windows_cxx("msvc-14.2", "", image="cppalliance/dronevs2019", buildscript="drone", environment={"TOOLSET": "msvc-14.2", "CXXSTD": "14,17,20,latest"}, globalenv=globalenv),
  windows_cxx("msvc-14.3", "", image="cppalliance/dronevs2022:1", buildscript="drone", environment={"TOOLSET": "msvc-14.3", "CXXSTD": "14,17,20,latest"}, globalenv=globalenv)
    ]

# from https://github.com/boostorg/boost-ci
load("@boost_ci//ci/drone/:functions.star", "linux_cxx", "windows_cxx", "osx_cxx")