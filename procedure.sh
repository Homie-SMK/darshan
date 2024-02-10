./prepare.sh
cd darshan_runtime
./configure --with-log-path=/darshan-logs --with-jobid-env=SLURM_JOBID CC=mpicc
make
make install
