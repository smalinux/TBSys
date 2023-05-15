FROM ubuntu
LABEL Description="TBSys Description"

ENV HOME /root

# Packages or dep
RUN apt-get update && apt-get -y --no-install-recommends install cmake build-essential sqlite3 libsqlite3-dev sqlite3

# COPY the files from OS
COPY * ./tbsys

# Commands
CMD mkdir tbsys; cd tbsys; make; ./tbsys
