# INSTALL

## Complie

Linux x64, test on virtualbox/vagrant ubuntu 14.04 x86_x64

- hdf5 build failed on vagrant linux with clang, clang can not recognise window folder file format, change to gnu c/c++
- if use virtualbox/vagrant windows shared folder, hdf5 need root priority to create symbol link
- numpy need to be installed, a wrapper library for numpy is created for calling
- ponyc needs pcre2-8(use pcre2-10) with Regex, build and install manually, then add the lib path to makefile https://github.com/ponylang/ponyc/issues/221
- install http://czmq.zeromq.org/ & install libzmq