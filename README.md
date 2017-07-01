# Pegasus

A trade platform, fast, safe, extensible, friendly

### Help

- open issue: https://github.com/17studio/Pegasus/issues
- community: https://github.com/17studio/Pegasus/wiki/Group

### Design

see https://github.com/17studio/Pegasus/wiki

### Complie

Linux x64, test on vagrant ubuntu 14.04 x86_x64

- hdf5 build failed on vagrant linux with clang, clang can not recognise window folder file format, change to gnu c/c++
- if use vagrant window folder, hdf5 need root priority to create symbol link
- numpy need to be installed, a wrapper library for numpy is created for calling
