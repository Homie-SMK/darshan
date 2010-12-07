#!/bin/bash

PREFIX=/soft/apps/darshan-2.0.2
LOGDIR=/pvfs-surveyor/logs/darshan

# darshan job summary
cd ../
install -d $PREFIX
install -d $PREFIX/bin
install -d $PREFIX/lib
install -d $PREFIX/lib/TeX
install -d $PREFIX/lib/Number
install -d $PREFIX/lib/Number/Bytes
install -d $PREFIX/share
install -m 755 util/bin/darshan-job-summary.pl $PREFIX/bin/
install -m 644 util/lib/TeX/Encode.pm $PREFIX/lib/TeX/
install -m 644 util/lib/Number/Bytes/Human.pm $PREFIX/lib/Number/Bytes
install -m 644 util/share/* $PREFIX/share/

# we also need to build darshan-parser in 2.0.2
./configure --with-mem-align=16 --with-log-path=$LOGDIR --prefix=$PREFIX --with-jobid-env=COBALT_JOBID CFLAGS="-O2" && make darshan-parser
install -m 755 darshan-parser $PREFIX/bin/

#gnuplot (new version with additional histogram support)
cd extern
tar -xvzf gnuplot-4.2.4.tar.gz
cd gnuplot-4.2.4
./configure --prefix $PREFIX && make &&  make install

