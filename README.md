# Telecom Billing System (TBSys)
Code is written as part of my technical interview assessment.

Telecom billing system is a software application used by telecommunication
companies to manage and automate their billing and revenue management
processes.

## Quick run using docker
Just one command:
``` bash
# https://hub.docker.com/r/smalinux/tbsys
docker run -it --rm smalinux/tbsys
```

## Build the source code:
Install dependencies:
``` bash
# cmake gcc
apt-get install cmake build-essential
# Installation of sqlite3
apt-get install sqlite3
# Installation of sqlite3.h
apt-get install libsqlite3-dev
```
build:
``` bash
git clone https://github.com/smalinux/TBSys
cd TBSys
make
```
run:
``` bash
./tbsys
```

Automate formatting the code:
``` bash
astyle -r -xb -s3 -p -xg -c -k1 -W1 -H \*.c \*.h
```

Generate docs
``` bash
doxygen
```


## TODO
- [x] Dockerizing TBSys
- [x] Doxygen Support
- [x] `astyle`
- [x] Input Validation (Almost done. skip it for now.)
- [ ] Add unit tests
- [x] logging support using `rxi/log`
- [x] Use library to create formatted ASCII tables for console applications
- [ ] Add `rxi/log` as git subtree (trivial. skip it for now.)
- [ ] Add `fort.ch` as git subtree (trivial. skip it for now.)
- [ ] Add unit test to check if `rxi/log` is working (trivial. skip it for now.)
- [ ] Split `tbsys.[ch]` to more logical modules. (Leave it for now!) db.ch, message.ch, etc
- [ ] Support argc & argv to invoke `tbsys` operations as a script (trivial. skip it)
- [ ] Use [IWYU](https://github.com/include-what-you-use/include-what-you-use) [Leave it for now!]
- [ ] Add GitHub actions for CI/CD
- [ ] Use OOP framework like [Maloc](http://fetk.org/codes/maloc/api/html/index.html)


<!--

`




$ sqlite3
-------------------
https://zetcode.com/db/sqlitec/



-->
