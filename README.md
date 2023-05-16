# Telecom Billing System (TBSys)

Simple CLI project was written as part of my technical interview assessment.

Video Demo: https://asciinema.org/a/AgRCZs0bk6eDptj9kPOe81Cwb

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
# cmake gcc sqlite3 sqlite3.h criterion
apt-get install cmake build-essential sqlite3 libsqlite3-dev libcriterion-dev
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

# or

./tbsys 2> /dev/null # skip logs
```
run unit tests:
``` bash
make test
```

automate formatting the code:
``` bash
astyle -r -xb -s3 -p -xg -c -k1 -W1 -H \*.c \*.h
```

Generate docs
``` bash
doxygen
```

## TODO
- [x] Sqlite DB support
- [x] Use library to create formatted ASCII tables for console applications
- [x] Dockerizing TBSys
- [x] Doxygen support
- [x] `astyle` support
- [x] Input Validation (Almost done. skip it for now.)
- [x] Check scanf return value: https://stackoverflow.com/q/7271939/5688267
- [ ] Unit testing using `criterion` unit test framework
- [x] logging support using `rxi/log`
- [ ] Add `rxi/log` as git subtree (trivial. skip it for now.)
- [ ] Add `fort.ch` as git subtree (trivial. skip it for now.)
- [ ] Add unit test to check if `rxi/log` is working (trivial. skip it for now.)
- [ ] Split `tbsys.[ch]` to more logical modules. (Leave it for now!) db.ch, message.ch, etc
- [ ] Support argc & argv to invoke `tbsys` operations as a script (trivial. skip it)
- [ ] Use [IWYU](https://github.com/include-what-you-use/include-what-you-use) (Leave it for now!)
- [ ] Add GitHub actions for CI/CD
- [ ] Use OOP framework like [Maloc](http://fetk.org/codes/maloc/api/html/index.html)


<!--

`
sometimes `make --debug=b` is nice for debugging the Makefile.




$ sqlite3
-------------------
https://zetcode.com/db/sqlitec/



-->
