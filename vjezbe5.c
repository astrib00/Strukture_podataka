//kod koji smo pisali na satu tijekom vjezbi
//napisat main funkciju doma

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _StackElement;
typedef struct _StackElement* Position;

typedef struct _StackElement
{
	double number;
	Position next;
} StackElement;

Position CreateElement(double number);
int InsertAfter(Position position, Position newElement);
int Push(Position head, double number);
int DeleteAfter(Position position);
int Pop(double* resultDestination, Position head);
int PerformOperation(Position head, char operation);
int CalculatePostfixFromFile(double* resultDestination, char* fileName);
int DeleteAll(Position head);
int InputFileName(char* fileName);
char* ReadFromFile(char *fileName);

int main(int argc, char* argv[]) 
{
	char fileName[70] = { 0 };
	double postfixResult = 0;

	InputFileName(fileName);

	if (CalculatePostfixFromFile(&postfixResult, fileName) == EXIT_SUCCESS)
	{
		printf("The result is: %.2lf \n", postfixResult);
		return EXIT_SUCCESS;
	}

	else
		return 0;
}

int InputFileName(char* fileName)
{
	printf("Enter the file name: \n");
	scanf(" %s", fileName);

	return EXIT_SUCCESS;
}

char* ReadFromFile(char *fileName)
{
    FILE *fp = NULL;
    int fileLength;
    char *buffer = NULL;

    fp = fopen(fileName, "rb");

    if(!fp){
        perror("Can't opne file!\n");
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    fileLength = ftell(fp);
    buffer = (char*)calloc(fileLength+1, sizeof(char));

    if(!buffer){
        perror("Can't allocate memory!\n");
        return NULL;
    }
    rewind(fp);

    fread(buffer, sizeof(char), fileLength, fp);
    printf("buffer: %s\n", buffer);
    fclose(fp);
    return buffer;
}

Position CreateElement(double number)
{
	Position newElement = NULL;

	newElement = (Position)malloc(sizeof(StackElement));

	if(!newElement)
	{
		perror("Can't allocate memory!\n");
		return NULL;
	}

	newElement->number = number;
	newElement->next = NULL;

	return newElement;
}

int InsertAfter(Position position, Position newElement)
{
	newElement->next = position->next;
	position->next = newElement;

	return 0;
}

int Push(Position head, double number)
{
	Position newElement = NULL;

	newElement = CreateElement(number);
	if(!newElement)
	{
		return -1;
	}

	InsertAfter(head, newElement);

	return 0;
}

int DeleteAfter(Position position)
{
	Position temp = position->next;

	if(!temp)
	{
		return 0;
	}

	position->next = temp->next;
	free(temp);

	return 0;
}

int Pop(double* resultDestination, Position head)
{
	Position first = head->next;

	if(!first)
	{
		perror("Postfix not valid! please check your file!\n");
		return -1;
	}

	*resultDestination = first->number;
	DeleteAfter(head);

	return 0;
}

int PerformOperation(Position head, char operation)
{
	double operand1 = 0;
	double operand2 = 0;
	double result = 0;
	int status1 = 0;
	int status2 = 0;

	status2 = Pop(&operand2, head);
	if (status2 != 0)
	{
		return -1;
	}

	status1 = Pop(&operand1, head);
	if (status1 != 0)
	{
		return -2;
	}

	switch(operation)
	{
		case '+':
			result = operand1 + operand2;
			break;
		case '-':
			result = operand1 - operand2;
			break;
		case '*':
			result = operand1 * operand2;
			break;
		case'/':
			if(operand2 == 0)
			{
				perror("Cannot divide with zero!\n");
				return -3;
			}

			result = operand1 / operand2;
			break;
		default:
			printf("This operation is not supported yet!"
				"Please pay and we will implement this operation!\n");
			return -4;
	}

	return Push(head, result);
}

int CalculatePostfixFromFile(double* resultDestination, char* fileName)
{
	FILE* file = NULL;
	int fileLength = 0;
	char* buffer = NULL;
	char* currentBuffer = NULL;
	int numBytes = 0;
	int number = 0;
	int status = 0;
	char operation = 0;
	StackElement head;
	head.number = 0;
	head.next = NULL;

	file = fopen(fileName, "rb"); //"rb"->citaj binarno!!

	if(!file)
	{
		perror("Can't open file!\n");
		return -1;
	}

	fseek(file, 0, SEEK_END);
	fileLength = ftell(file);

	buffer = (char*)calloc(fileLength + 1, sizeof(char)); // + 1 je radi '/0' na kraju stringa

	if(!buffer)
	{
		perror("Can't allocate memory!\n");
		return -2;
	}

	rewind(file);

	fread(buffer, sizeof(char), fileLength, file);
	printf("|%s|\n", buffer);
	fclose(file);

	currentBuffer = buffer;

	while(strlen(currentBuffer) > 0)
	{
		status = sscanf(currentBuffer, " %lf %n", &number, &numBytes);

		if (status == 1)
		{
			status = Push(&head, number);

			if (status != EXIT_SUCCESS)
			{
				free(buffer);
				DeleteAll(&head);
				return -3;
			}
			currentBuffer += numBytes;
		}
		else
		{
			sscanf(currentBuffer, " %c %n", &operation, &numBytes);

			status = PerformOperation(&head, operation);

			if (status != EXIT_SUCCESS)
			{
				free(buffer);
				DeleteAll(&head);
				return -4;
			}
			currentBuffer += numBytes;
		}
	}

	free(buffer);

	status = Pop(resultDestination, &head);

	if(status != 0)
	{
		DeleteAll(&head);
		return -5;
	}

	if(head.next)
	{
		printf("Postfix not valid!! Please check your file!\n");
		DeleteAll(&head);
		return -6;
	}

	return 0;
}

int DeleteAll(Position head)
{
	while (head->next)
	{
		DeleteAfter(head);
	}

	return 0;
}
