# 실행환경
- google colab (hardware acclerator GPU)
- nvcc compiler

# compile & execution
i) compile
nvcc -o [execution file] [CPU code(*.c) / GPU code(*.cu)]
2) execution
./[execution file]


# 실행 예시
NUM_ELEMENT 값을 바꿔서 array의 크기를 조절할 수 있다.
output은 array의 max value와 실행 시간이다.

ex.
MAX NUM : -
EXEC TIME : - mx

* CUDA2.ipynb file은 실행환경을 보여주기 위하여 첨부