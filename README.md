
# Quick run using docker
Just one command:
``` bash
# https://hub.docker.com/r/smalinux/tbsys
docker run -it --rm smalinux/tbsys
```




``` bash
# Installation of sqlite3
apt install sqlite3
# Installation of sqlite3.h
apt install libsqlite3-dev
```

If you want to automate formatting your code, the following command gives you a good baseline of how it should look:
``` bash
astyle -r -xb -s3 -p -xg -c -k1 -W1 -H \*.c \*.h
```

Generate docs
``` bash
doxygen
```


## TODO
- [ ] Write perfect README to improve SEO
- [ ] Add unit tests
- [x] `astyle`
- [ ] Add `rxi/log` as git subtree
- [ ] Add unit test to check if `rxi/log` is working
- [x] Add log_info() to improve the code usability
- [ ] Split `tbsys.[ch]` to more logical modules. (Leave it for now!) db.ch, message.ch, etc
- [ ] Add GitHub actions for CI/CD
- [ ] Support argc & argv to invoke `tbsys` operations as a script
- [x] Dockerizing TBSys
- [x] Doxygen Support
- [x] Input Validation (Almost done. skip it for now.)
- [ ] Use [IWYU](https://github.com/include-what-you-use/include-what-you-use) [Leave it for now!]
- [ ] Use OOP framework like [Maloc](http://fetk.org/codes/maloc/api/html/index.html)


<!--

`




$ sqlite3
-------------------
https://zetcode.com/db/sqlitec/



-->
