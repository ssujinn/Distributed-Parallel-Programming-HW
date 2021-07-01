struct operand{
    int num1;
    int num2;
};

program CALCULATOR_PROG {
    version CALCULATOR_VERS {
	int add(operand) = 1;
	int sub(operand) = 2;
	int mul(operand) = 3;
	int div(operand) = 4;
	int pow(operand) = 5;
    } = 1;
} = 0x20171640;
