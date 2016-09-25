monograph
=========

A program for creating diagrams for fixed-width character output.


Building monograph
------------------
Clone the repository and initialize submodules, then generate the
`configure` script, configure and build.

    git clone https://github.com/donmccaughey/monograph.git
    cd monograph
    git submodule init
    git submodule update --init --recursive
    autoreconf -i
    mkdir tmp && cd tmp
    ../configure && make check


License
-------
`monograph` is made available under a BSD-style license; see the LICENSE
file for details.

