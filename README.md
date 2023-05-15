


``` bash
# Installation of sqlite3
apt install sqlite3
# Installation of sqlite3.h
apt install libsqlite3-dev
```


## TODO
- [ ] Use [IWYU](https://github.com/include-what-you-use/include-what-you-use) [Leave it for now!]
- [ ] Add `rxi/log` as git subtree
- [ ] Add unit test to check if `rxi/log` is working
- [x] Add log_info() to improve the code usability
- [ ] Split `tbsys.[ch]` to more logical modules. (Leave it for now!) db.ch, message.ch, etc
- [ ] Add GitHub actions for CI/CD
- [ ] Support argc & argv to invoke `tbsys` as a script

<!--

`
https://cloudinfrastructureservices.co.uk/how-to-install-db-browser-for-sqlite-in-ubuntu-server-20-04/


GitHub repo SEO:
-------------------
description: tbsys written as technical interview test


$ sqlite3
-------------------
read more about this tool
integrate it with you bash scripts


sqlite> .help


create db using bash
---------------------
$ sqlite3 test.db




-->
