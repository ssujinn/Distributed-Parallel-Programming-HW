1) build 방법
prefixSum_blck.c prefixSum_nblck.c prefixSum_Scan.c의 경우
mpicc [file명] -o [exec파일명]
을 통해 컴파일 한 후
mpiexec -np [num of processors] -mca btl ^openib -hostfile hosts ./[exec file]
을 통해 실행할 수 있다.

prefixSum_serial.c 파일의 경우
gcc [file name] -o [exec filename]
으로 컴파일 한 후
./[exec file] [num of procs]
을 통해 실행할 수 있다.

2) synopsis
- blocking program
mpicc prefixSum_blck.c -o blck
mpiexec -np numofprocs -mca btl ^openib -hostfile hosts ./blck

- nonblocking program
mpicc prefixSum_nblck.c -o nblck
mpiexec -np numofprocs -mca btl ^openib -hostfile hosts ./nblck

- MPI_Scan program
mpicc prefixSum_Scan.c -o scan
mpiexec -np numofprocs -mca btl ^openib -hostfile hosts ./scan

- serial program
gcc prefixSum_serial.c -o serial
./serial numofprocs

3) 실행 예시
<Before>
num 1> ***
...
num i> ***

<After>
num 1> ###
...
num i> ###

total time : ------- ms
