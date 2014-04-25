tiny-ci
=======

A tiny light-weight continuous integration server

Compile instructions
------------------

The files can be compiled using make with the following command.

```bash
make setup
```

This will create a binary file called '*tiny-ci*' in the bin directory which can be called from the command line. It will also create a directory in '*~/.tiny-ci*' which is used to store the settings for tiny-ci and finally will also create a python script called '*~/.tiny-ci/processFail.py*'. This is the script which defines what happens when a test fails. After installation it is suggested that you edit this file and enter your SMTP email settings.

For help on the options avalible you can run

```bash
tiny-ci help
```

