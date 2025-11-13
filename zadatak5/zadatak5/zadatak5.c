#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX (1024)

typedef struct stackEl* position;
typedef struct stackEl {
	double number;
	position next;
}stackEl;

int readFile(position top, double* result, char* postfix);
int push(double number, position top);
int checkStackAndExtractResult(position top, char oper);
int pop(position top);

int main(){
	stackEl head = { 0, NULL };
	char postfix[MAX] = { 0 };
	double result = 0;

	if (readFile(&head, &result, postfix) == EXIT_SUCCESS)
		printf("rezultat je %lf\n", result);

	return EXIT_SUCCESS;
}


int readFile(position top, double* result, char* postfix) {
	FILE* fileName = NULL;
	int numBytes = 0, status = 0;
	double number = 0;
	char oper = '\0';

	fileName = fopen("postfix.txt", "r");

	if (!fileName) {
		printf("File not opened\n");
		return EXIT_FAILURE;
	}

	fgets(postfix, MAX, fileName);
	while (strlen(postfix) > 0) {
		status = sscanf(postfix, " %lf %n", &number, &numBytes);
		if (status == 1) {
			push(number, top);
		}
		else {
			sscanf(postfix, " %c %n", &oper, &numBytes);
			checkStackAndExtractResult(top, oper);
		}
		postfix += numBytes;
	}

	*result = pop(top);
	fclose(fileName);
	return EXIT_SUCCESS;

}

int push(double number, position top) {
	position newElement = NULL;
	newElement = malloc(sizeof(stackEl));

	if (!newElement) {
		printf("Allocation for newElement failed\n");
		return EXIT_FAILURE;
	}

	newElement->number = number;
	newElement->next = top->next;
	top->next = newElement;

	return EXIT_SUCCESS;
}

int pop(position top) {

	position toDelete = NULL;
	int poppedResult = 0;

	toDelete = top->next;
	top->next = toDelete->next;

	poppedResult = toDelete->number;

	free(toDelete);

	return poppedResult;
}

int checkStackAndExtractResult(position top, char oper) {
	double operandRes = 0;
	int counter = 0;

	switch (oper) {

	case '+':
		operandRes = 0; 
			for (counter = 0; counter < 2; counter++) {
				operandRes += top->next->number;
				pop(top);
			}
		push(operandRes, top);
		break;

	case '-':

		operandRes = 0;
		for (counter = 0; counter < 2; counter++) {
			if (operandRes == 0)
				operandRes = top->next->number;

			else
				operandRes = top->next->number - operandRes;

			pop(top);
		}


		push(operandRes, top);

		break;

	case '*':

		operandRes = 0;

		for (counter = 0; counter < 2; counter++) {


			if (operandRes == 0)
				operandRes = top->next->number;

			else
				operandRes *= top->next->number;

			pop(top);
		}

		push(operandRes, top);

		break;

	case '/':

		operandRes = 0;

		for (counter = 0; counter < 2; counter++) {

			if (operandRes == 0)
				operandRes = top->next->number;

			else
				operandRes = top->next->number / operandRes;

			pop(top);
		}

		push(operandRes, top);

		break;

	default:
		printf("\nOperation %c not supported yet!\r\n", oper);
		return EXIT_FAILURE;

	}


	return EXIT_SUCCESS;
}
