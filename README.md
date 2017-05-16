# The ezV24 Library

The goal of the *ezV24* library is to provide an easy to use interface to the
serial ports of an linux/unix system. At the current stage, it comes with
rudimentary support for the CYGWIN toolchain (see
http://www.cygwin.com/). Due to the great work of the cygwin folks, only
minimal changes are needed. I know that that the cygwin-stuff isn't the final
stuff, but it's a first step and it works.


## HOME ON THE NET

The project it hosted at github. You can find the latest release of the
*ezV24* library at https://github.com/joede/libezV24. If you have some
questions, feel free to write me an email <github@jdesch.de>. To
ease the mail handling, please put a `[ezV24]` into the subject line of the
mail.


## INSTALLATION

To install the libary, just unpack the archive (you may have done this ;) and
execute the following steps:

1. compile the library by typing `make`. The result should be a executable
   shared library named `libezV24.so.*` and the sample `test-v24`. To see what
   `test-v24` do, you must have a look into the source code.

2. become root and install the library and the header files. This call will
   update you ldd-cache too! Note that this doen's install the html
   documentation! I'm not shure where to install it, so you can put it into
   your prefered directory.

        $ su
        # make install
        # exit

    Some systems don't use `/usr/local/lib` as possible path for libraries. In
    this case, you should add `/usr/local/lib` to `/etc/ld.so.conf` before
    calling `make install`!



## UPDATE

Previous releases of *ezV24* have installed the unnecessary header
ezV24_conf.h. If you update to V0.1.0, you should either call the old
Makefile with `make uninstall` or remove the file manually.


## USAGE

To use the libary for your own applications, you have to include the main
header:

```` .C
#include <ezV24/ezV24.h>
````

To link the shared library to your application, you just have to specify the
basename of the library with `-lezV24`. The whole call could look like this:

```` .shell
gcc -o foo foo.c -lezV24
````

For more information, just have a look into the html documentation.


## PACKAGING

At the time being, ezV24 supports the debian package format .deb and the Red
Hat format RPM. Both package type are `unofficial'. This means, these
packages are not build by official members of the corresponding
organizations.

The spec file for RPM's is made by Pascal Bleser <guru@unixtech.be>!
Thanx. Binary build for SuSE-Linux could be found at
http://guru.unixtech.be/rpm/packages/Development/libezV24/.  Note that I
neither build RPM's nor test the RPM build process. If you have any questions
about RPM's, please contact Pascal Bleser.


--
Joerg Desch
