1) build
make -f Makefile.calculator
로 컴파일한 후
cspro2에서 ./calculator_server를 실행한 후
cspro1에서 ./calculator_client cspro2.sogang.ac.kr
을 실행한다

2) synopsis
make -f Makefile.calculator

cspro2
./calculator_server

cspro1
./calculator_client cspro2.sogang.ac.kr

3) 실행예시
assignment> test
[계산할 문자열]
The answer is **
assignment> exit

-> 종료
