tiny-ci
=======

A tiny light-weight continuous integration server

Compile instructions
------------------

The files can be compiled using make with the following command.

```bash
make
```

This will create a binary file called '*tiny-ci*' in the bin directory which can be called from the command line. 

For help on the options avalible you can run

```bash
tiny-ci help
```


TODO
-------

+ Add support for **bash** tests
+ Add support for **maven** tests
+ Add email alerts
+ Store histories of tests
+ Consider moving to a database rather than files (SQLite)
+ Store build histories (with compiled files accessable?)
