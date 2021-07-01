# 실행환경
- google colab (hardware acclerator GPU)
- nvcc compiler

# compile & execution
i) compile
nvcc -o [execution file] [GPU code(*.cu)]
2) execution
./[execution file]


# 실행 예시
output은 array의 크기가 크기 때문에 출력하지 못하였다.
출력은 해당 program의 elapsed time이다.

ex.
EXEC TIME : - ms

* CUDA1.ipynb file은 실행환경을 보여주기 위하여 첨부