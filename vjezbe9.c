#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Node;
typedef struct Node* position;

typedef struct Node
{
	int number;
	position left;
	position right;
} node;

struct Stog; //brojeve iz stabla ucitavamo na stog, a na stogu sortiramo i upisujemo u datoteku
typedef struct Stog* positionSt;
typedef struct Stog
{
	int el;
	positionSt next;
} stog;

position noviElement(int broj);
position ucitajBrojeve(position root, int brojevi[]);
position insert(position root, position novi);
int printInOrder(position root, positionSt head);
int Push(positionSt head, int broj);
positionSt noviElementNaStogu(int broj);
int Pop(positionSt head);
int DeleteAfter(positionSt head);
int pisiDatoteku(positionSt head, char* datoteka);
position replace(position root);
int izracunajSumu(position root, int* suma);
position brisiStablo(position root);
position randomStablo(position root);

int main()
{ 
	stog head;
	position root;
	int broj;
	int brojevi[10] = {2, 5, 7, 8, 11, 1, 4, 2, 3, 7};
	head.el = 0;
	head.next = NULL;
	root = NULL;
	broj = 0;

	root = ucitajBrojeve(root, brojevi);

	printInOrder(root, &head);
	broj = pisiDatoteku(&head, "StabloA.txt");

	root = replace(root);

	printInOrder(root, &head);
	broj = pisiDatoteku(&head, "StabloB.txt");

	root = brisiStablo(root);
	root = randomStablo(root);

	printInOrder(root, &head);
	broj = pisiDatoteku(&head, "StabloC.txt");

	return 0;
}

position noviElement(int broj)
{
	position noviEl = (position)malloc(sizeof(node));
	if(!noviEl)
	{
		printf("Memorija nije alocirana!\n");
		return NULL;
	}

	noviEl->number = broj;
	noviEl->left = NULL;
	noviEl->right = NULL;

	return noviEl;
}

position ucitajBrojeve(position root, int brojevi[])
{
	position novi = NULL;
	int i = 0;

	for(i=0;i<10;i++)
	{
		novi = noviElement(brojevi[i]);

		root = insert(root, novi); //insert funkcija mora vracat root pa zato ovde pridjeljujemo vrijednost
	}

	return root;
}

position insert(position root, position novi)
{
	if(root == NULL)
		root = novi;

	else if (novi->number > root->number) 
		root->left = insert(root->left, novi);

	else if (novi->number <= root->number)
		root->right = insert(root->right, novi);

	return root;
}

int printInOrder(position root, positionSt head)
{
	if(root)
	{
		printInOrder(root->right, head);
		Push(head, root->number); //Pushamo elemente na stog u inorder redoslijedu
		printInOrder(root->left, head);
	}

	return 0;
}

int Push(positionSt head, int broj)
{
	positionSt novi = NULL;

	novi = noviElementNaStogu(broj);
	if(!novi) 
		return -1;

	novi->next = head->next;
	head->next = novi;

	return 0;
}

positionSt noviElementNaStogu(int broj)
{
	positionSt novi = NULL;

	novi = (positionSt)malloc(sizeof(stog));
	if(!novi)
	{
		printf("Memorija nije uspjesno alocirana!\n");
		return NULL;
	}

	novi->el = broj;
	novi->next = NULL;

	return novi;
}

int Pop(positionSt head)
{
	int broj;

	broj = head->next->el;

	DeleteAfter(head);

	return broj; //trebat ce nam broj prilikom pisanja u datoteku
}

int DeleteAfter(positionSt head)
{
	positionSt temp = NULL;

	temp = head->next;
	head->next = temp->next;
	free(temp);

	return 0;
}

int pisiDatoteku(positionSt head, char* datoteka)
{
	FILE* dat = NULL;
	dat = fopen(datoteka, "w");
	if(!dat)
	{
		printf("Datoteka nije uspjesno otvorena!\n");
		return -1;
	}

	while(head->next != NULL)
		fprintf(dat, "%d ", Pop(head)); //u file se upise onaj broj koji je skinut sa stoga

	fclose(dat);
	return 0;
}

position replace(position root)
{
	int suma = 0;

	izracunajSumu(root, &suma);
	root->number = suma;

	if(root->left)
		root->left = replace(root->left);

	if(root->right)
		root->right = replace(root->right);

	return root;
}

int izracunajSumu(position root, int* suma)
{
	if(root->left)
		(*suma) += izracunajSumu(root->left, suma);

	if(root->right)
		(*suma) += izracunajSumu(root->right, suma);

	return root->number;
}

position brisiStablo(position root)
{
	if(root->right)
		root->right = brisiStablo(root->right);

	if(root->left)
		root->left = brisiStablo(root->left);

	free(root);

	return NULL;
}

position randomStablo(position root)
{
	position novi = NULL;

	int brojevi[10] = {0};
	int i = 0;

	srand(time(NULL));

	for(i = 0;i < 10; i++)
	{
		brojevi[i] = (rand() % (90 - 10 + 1)) + 10;
		novi = noviElement(brojevi[i]);

		root = insert(root, novi);
	}

	return root;
}
