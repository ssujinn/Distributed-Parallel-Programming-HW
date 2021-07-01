# 실행환경
- cspro2
- gcc compiler

# compile & execution
i) compile
gcc -g -Wall -fopenmp -o [execution file] palindrome.c
2) execution
./[execution file] [num of threads] [input file] [output file]

# input
input file은 words.txt 기준

# output
output file은 palindrome word A의 reverse가 word B라고 할 때
	(word A) (word B)
의 format으로 작성 (word A == word B일 경우에도 동일)

단, (word B) (word A)와 같이 중복되는 경우는 작성하지 않음

ex.
civic civic
dab bad
---------------------
bad dab (x)

* palindrome.txt는 output file 예시이다.
