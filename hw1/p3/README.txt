1) build 방법
- sequential program
gcc img_seq.c -o [execfile명]
으로 컴파일한 후
./[execfile명]
으로 실행한다.

- parallel program
mpicc img_par.c -o [execfile명]
으로 컴파일한 후
mpiexec -np [numofprocs] -mca btl ^openib -hostfile hosts ./[execfile]
로 실행한다.

2) synopsis
- sequential program
gcc img_seq.c -o seq
./seq

- parallel program
mpicc img_par.c -o par
mpiexec -np numofprocs -mca btl ^openib -hostfile hosts ./par

3) 실행예시
filename? [filename]
Select mode
1. flip an image horizontally
2. grayscale (average)
3. smooth image
[1 or 2 or 3]

-> get image processed in this folder

4) ppm viewer
Linux -> vim으로 contents 확인
Window -> 꿀뷰 사용
