/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "calculator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main (int argc, char *argv[])
{
	char *host;
	CLIENT *clnt;
	char line[100], exp[100];
	int i = 0;
	int idx1, idx2, idx3, idx4, expidx = 0, opflag = 0, numflag = 0;
	char numtmp[10], optmp[10];
	int numstack[100];
	char opstack[100];
	int numtop = -1, optop = -1;


	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}

	clnt = clnt_create(argv[1], CALCULATOR_PROG, CALCULATOR_VERS, "udp");

	if (clnt == (CLIENT*)NULL){
	    clnt_pcreateerror(argv[1]);
	    exit(1);
	}

	while (1) {
	char cmd[100];
	printf("assignment> ");
	scanf("%s", cmd);

	if (!strcmp(cmd, "exit"))
	    break;
	else if (!strcmp(cmd, "test")){
	    int breakflag = 0;
	memset(line, 0, sizeof(line));
	memset(numstack, 0, sizeof(numstack));
	memset(opstack, 0, sizeof(opstack));
	i = idx1 = idx2 = idx3 = idx4 = expidx = opflag = numflag = 0;
	numtop = optop = -1;
	getchar();
	fgets(line, sizeof(line), stdin);
	memset(exp, 0, sizeof(exp));
	while (line[i] != '\n'){
	    if (breakflag == 1)
		break;
	    if (line[i] < '0' || line[i] > '9'){
		if (opflag == 0 && numflag == 1){
		    idx1 = idx2 = i;
		    opflag = 1;
		    numflag = 0;
		    memset(numtmp, 0, sizeof(numtmp));
		    strncpy(numtmp, line + idx3, idx4 - idx3 + 1);
		    sprintf(exp + expidx, " %s", numtmp);
		    expidx += (idx4 - idx3 + 2);
		}
		else if (opflag == 1 && numflag == 0)
		    idx2 = i;
		else {
		    printf("wrong expression!\n");
		    breakflag = 1;
		    break;
		}
	    }
	    else {
		if (numflag == 0){
		    if (opflag == 1){
			memset(optmp, 0, sizeof(optmp));
			strncpy(optmp, line + idx1, idx2 - idx1 + 1);
			if (!strcmp(optmp, "+")){
			    if (optop == -1){
				optop++;
				opstack[optop] = '+';
			    }
			    else {
				sprintf(exp + expidx, " %c", opstack[optop]);
				expidx += 2;
				opstack[optop] = '+';
			   }
			}
			else if (!strcmp(optmp, "-")){
			    if (optop == -1){
				optop++;
				opstack[optop] = '-';
			    }
			    else {
				sprintf(exp + expidx, " %c", opstack[optop]);
				expidx += 2;
				opstack[optop] = '-';
			    }
			}
			else if (!strcmp(optmp, "*")){
			    if (optop == -1){
				optop++;
				opstack[optop] = '*';
			    }
			    else if (opstack[optop] == '*' || opstack[optop] == '/' || opstack[optop] == '^'){
				sprintf(exp + expidx, " %c", opstack[optop]);
				expidx += 2;
				opstack[optop] = '*';
			    }
			    else {
				optop++;
				opstack[optop] = '*';
			    }
			}
			else if (!strcmp(optmp, "/")){
			    if (optop == -1){
				optop++;
				opstack[optop] = '/';
			    }
			    else if (opstack[optop] == '*' || opstack[optop] == '/' || opstack[optop] == '^'){
				sprintf(exp + expidx, " %c", opstack[optop]);
				expidx += 2;
				opstack[optop] = '/';
			    }
			    else {
				optop++;
				opstack[optop] = '/';
			    }

			}
			else if (!strcmp(optmp, "**")){
			    if (optop == -1){
				optop++;
				opstack[optop] = '^';
			    }
			    else if (opstack[optop] == '^'){
				sprintf(exp + expidx, " %c", opstack[optop]);
				expidx += 2;
				opstack[optop] = '^';
			    }
			    else {
				optop++;
				opstack[optop] = '^';
			    }
			}
			else {
			    printf("wrong expression!\n");
			    breakflag = 1;
			    break;
			}
		    }
		    opflag = 0;
		    numflag = 1;
		    idx3 = idx4 = i;
		}
		else
		    idx4 = i;
	    }
	    i++;
	}

	if (opflag == 1){
	    printf("wrong expression!\n");
	    breakflag = 1;
	}

	if (breakflag == 1)
	    continue;
	
	if (line[i] == '\n'){
	    memset(numtmp, 0, sizeof(numtmp));
	    strncpy(numtmp, line + idx3, idx4 - idx3 + 1);
	    sprintf(exp + expidx, " %s", numtmp);
	    expidx += (idx4 - idx3 + 2);
	}

	while (optop >= 0){
	    sprintf(exp + expidx, " %c", opstack[optop]);
	    optop--;
	    expidx += 2;
	}

	i = 0;
	while (1){
	    if (breakflag == 1)
		break;
	    if (exp[i] == 0 || exp[i] == ' '){
		if (exp[i + 1] >= '0' && exp[i + 1] <= '9'){
		    idx3 = idx4 = i + 1;
		    i++;
		    while (exp[i] != ' ' && exp[i] != 0){
			idx4 = i;
			i++;
		    }
		    i--;
		    numtop++;
		    memset(numtmp, 0, sizeof(numtmp));
		    strncpy(numtmp, exp + idx3, idx4 - idx3 + 1);
		    numstack[numtop] = atoi(numtmp);
		}
		else {
		    operand send;
		    int *output;
		    send.num2 = numstack[numtop--];
		    send.num1 = numstack[numtop--];
		    i++;
		    switch (exp[i]){
			case '+':
			    numtop++;
			    output = add_1(&send, clnt);
			    if (output == (int*)NULL){
				clnt_perror(clnt, "call failed");
			    }
			    numstack[numtop] = *output;
			    break;
			case '-':
			    numtop++;
			    output = sub_1(&send, clnt);
			    if (output == (int*)NULL){
				clnt_perror(clnt, "call failed");
			    }
			    numstack[numtop] = *output;

			    break;
			case '*':
			    numtop++;
			    output = mul_1(&send, clnt);
			    if (output == (int*)NULL){
				clnt_perror(clnt, "call failed");
			    }
			    numstack[numtop] = *output;

			    break;
			case '/':
			    output = div_1(&send, clnt);
			    if (output == (int*)NULL){
				clnt_perror(clnt, "call failed");
			    }
			    if (*output == (1 << 30)){
				printf("division by zero!\n");
				breakflag = 1;
			    }
			    else{
				numtop++;
				numstack[numtop] = *output;
			    }
			    break;
			case '^':
			    numtop++;
			    output = pow_1(&send, clnt);
			    if (output == (int*)NULL){
				clnt_perror(clnt, "call failed");
			    }
			    numstack[numtop] = *output;
			    break;

		}
	    }

	    if (exp[i] == 0)
		break;
	    i++;
	    }
	}
	if (breakflag == 1)
	    continue;

	printf("The answer is %d\n", numstack[0]);
	}
	}

clnt_destroy(clnt);
exit (0);
}
