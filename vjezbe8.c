#include <stdio.h>
#include <stdlib.h>

struct node;
typedef struct node* position;
typedef struct node
{
	int number;
	position left;
	position right;
} Node;

position insert(position current, int el);
void printInOrder(position current);
void printPostorder(position current);
void printPreorder(position current);
void printLevelOrder(position current);  //jos malo proucit pa dovrsit ovu funkciju!!!
position find(position current, int number);
position deleteNode(position current, int number);
position createNode(int number);
position findMin(position current);
position findMax(position current);

int main()
{
	position root = NULL;

	int number, br, wantedNumber;

	char sign = 0;
	while (sign != 'e')
	{
		printf("\n Operacija:\n");
		printf("i - insert\np - print\nf - find element\nd - delete\ne - exit\n");
		scanf(" %c", &sign);

		switch(sign)
		{
			case 'i':
				printf("Unesite nekakav broj: \n");
				scanf("%d", &br);
				root = insert(root, br);
				break;

			case 'p':
				printInOrder(root);
				printPostorder(root);
				printPreorder(root);
				printLevelOrder(root);
				break;

			case 'd':
				printf("Unesite broj koji zelite obrisati: \n");
				scanf("%d", &br);
				root = deleteNode(root, br);
				break;

			case 'f':
				printf("Unesite broj koji zelite pronaci: \n");
				scanf("%d", &br);
				wantedNumber = find(root, br);
				if (wantedNumber != NULL)
					printf("Broj %d je uspjesno pronaden!\n", br);
				break;

			default:
				printf("GRESKA!! Unijeli ste nepostojecu naredbu! \n");
				break;
		}
	}


	return 0;
}

position insert(position current, int el)
{
	if(current == NULL)
	{
		current = (position)malloc(sizeof(Node));

		current->number = el;
		current->left = NULL;
		current->right = NULL;
	}

	else if(current->number > el)
	{
		current->left = insert(current->left, el);
	}
	else if(current->number < el) 
	{
		current->right = insert(current->right, el);
	}
	else 
	{
		printf("GRESKA!! Dupliciran clan! \n");
	}

	return current;
}

void printInOrder(position current)
{
	if(current == NULL)
		return;

	printInOrder(current->left);
	printf("%d ", current->number);
	printInOrder(current->right);
}

void printPostorder(position current)
{
	if(current == NULL)
		return;

	printPostorder(current->left);
	printPostorder(current->right);
	printf("%d ", current->number);
}

void printPreorder(position current)
{
	if(current == NULL)
		return;

	printf("%d ", current->number);
	printPreorder(current->left);
	printPreorder(current->right);
}

void printLevelOrder(position current)
{



}

position find(position current, int number)
{
	if(current == NULL)
		return NULL;

	if(current->number == number)
	{
		return current;
	}
	else if(current->number > number)
	{
		return find(current->left, number);
	}
	else
	{
		return find(current->right, number);
	}
}

position deleteNode(position current, int number)
{
	position tmp;

	if(current == NULL)
		return NULL;

	if(current->number == number)
	{
		if(current->left == NULL && current->right == NULL)
		{
			free(current);
			return NULL;
		}

		if(current->left != NULL)
		{
			tmp = findMax(current->left);
			current->number = tmp->number;
			current->left = deleteNode(current->right, tmp->number);
		}
		else if(current->right != NULL)
		{
			tmp = findMax(current->right);
			current->number = tmp->number;
			current->right = deleteNode(current->right, tmp->number);
		}
	}
	else if(current->number < number)
	{
		current->left = deleteNode(current->left, number);
	}
	else 
	{
		current->right = deleteNode(current->right, number);
	}

	return current;
}

position findMax(position current)
{
	if(NULL == current)
		return NULL;

	while(current->right != NULL)
		current = current->right;
}

position findMin(position current)
{
	if(current == NULL)
		return NULL;

	while(current->left != NULL)
	{
		current = current->left;
	}

	return current;
}

position createNode(int number)
{
	position p = NULL;

	p = (position)malloc(sizeof(Node));

	if(p == NULL)
	{
		printf("Greska u alokaciji memorije!!!\n");
		return NULL;
	}

	else
		p->number = number;
		p->left = NULL;
		p->right = NULL;
}
