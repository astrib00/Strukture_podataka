#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node;
typedef struct Node* positionNode;
typedef struct Node
{
	char ime[128];
	int brStanovnika;
	positionNode next;
} node;

struct Stablo;
typedef struct Stablo* position;
typedef struct Stablo
{
	char ime[128];
	position left;
	position right;
	positionNode head;
} stablo;

position citajStablo(char* filename, position root);
int citajDatoteku(char* filename, positionNode head);
int Sortiraj(positionNode novi, positionNode head);
int usporediGradove(positionNode novi, positionNode stari);
int InsertAfter(positionNode prev, positionNode novi);
position noviElementStablo(char* ime);
position unesiStablo(position novi, position current);
int pronadiGrad(positionNode head, int broj);
position pronadiDrzavu(position current, char* ime, int broj);
int ispisiListu(positionNode head);
int ispisiStablo(position current);

int main()
{
	position root = NULL;
	char ime[128] = {0};
	int broj;

	root = citajStablo("drzave.txt", root);

	printf("Unesite drzavu u kojoj se nalazi grad kojeg zelite pretraziti i zeljeni broj stanovnika.\n");
	scanf(" %s", ime);
	scanf("%d", &broj);

	pronadiDrzavu(root, ime, broj);

	return 0;
}

position citajStablo(char* filename, position root)
{
	char buffer[1024] = {0};
	FILE* dat = NULL;
	
	dat = fopen(filename, "r");
	if(!dat)
	{
		perror("Datoteka nije uspjesno otvorena!\n");
		return NULL;
	}

	while(!feof(dat))
	{
		char drzava[128] = {0};
		char ime[128] = {0}; //tu cemo ucitati ime datoteke za gradove odredene drzave
		position novi = NULL;

		fgets(buffer, 128, dat);
		if(sscanf(buffer, " %s %s", drzava, ime) == 2)
		{
			novi = noviElementStablo(drzava);
			citajDatoteku(ime, novi->head);
			root = unesiStablo(novi, root);
		}
	}
	fclose(dat);
	return root;
}

position noviElementStablo(char* ime)
{
	position novi = NULL;

	novi = (position)malloc(sizeof(stablo));
	if(!novi)
	{
		perror("Dinamicka alokacija memorije nije uspjela!\n");
		return NULL;
	}

	strcpy(novi->ime, ime);
	novi->head = (positionNode)malloc(sizeof(node)); //head za vezanu listu u tom cvoru stabla tj te drzave
	if(!(novi->head)) 
	{
		perror("Dinamicka alokacija memorije nije uspjela!\n");
		return NULL; 
	}

	novi->head->next = NULL;
	novi->left = NULL;
	novi->right = NULL;

	return novi;
}

position unesiStablo(position novi, position current)
{
	if(current == NULL)
		return novi;
	else if(strcmp(current->ime, novi->ime) > 0)
		current->left = unesiStablo(novi, current->left);
	else if(strcmp(current->ime, novi->ime) > 0)
		current->right = unesiStablo(novi, current->right);
	else
		free(novi);

	return current;
}


int citajDatoteku(char* filename, positionNode head) //kreira se lista gradova
{
	char buffer[1024] = {0};
	FILE* dat = NULL;

	dat = fopen(filename, "r");
	if(!dat)
	{
		perror("Datoteka nije uspjesno otvorena!\n");
		return -1;
	}

	while(!feof(dat))
	{
		positionNode novi = NULL;
		novi = (positionNode)malloc(sizeof(node));
		if(!novi)
		{
			perror("Alokacija nije uspjela!\n");
			return -2;
		}

		fgets(buffer, 128, dat);
		if(sscanf(buffer, " %d %d", novi->ime, &novi->brStanovnika) == 2)
		{
			Sortiraj(novi, head);
		}
	}
		fclose(dat);
		return 0;
	}

int Sortiraj(positionNode novi, positionNode head)
{
	positionNode temp = head;
	while(temp->next != NULL && usporediGradove(novi, temp->next) < 0)
	{
		temp = temp->next;
	}
	InsertAfter(temp, novi);

	return 0;
}

int usporediGradove(positionNode novi, positionNode stari)
{
	int broj = novi->brStanovnika - stari->brStanovnika;

	if(novi->brStanovnika == stari->brStanovnika)
	{
		broj = (strcmp(stari->ime, novi->ime) < 0);
	}
	return broj;
}

int InsertAfter(positionNode prev, positionNode novi)
{
	novi->next = prev->next;
	prev->next = novi;

	return 0;
}

position pronadiDrzavu(position current, char* ime, int broj)
{
	if(current == NULL)
	{
		printf("Datoteka je prazna!\n");
		return NULL;
	}

	else if (strcmp(current->ime, ime) < 0)
		current->right = pronadiDrzavu(current->right, ime, broj);
	else if (strcmp(current->ime, ime) > 0)
		current->left = pronadiDrzavu(current->left, ime, broj);
	else
	{
		printf("Gradovi koji imaju veci broj stanovnika: \n");
		pronadiGrad(current->head, broj);
	}
	return current;
}

int pronadiGrad(positionNode head, int broj)
{
	positionNode temp = head->next;

	while(temp != NULL)
	{
		if(temp->brStanovnika >= broj)
		{
			printf("Grad: %s   Broj stanovnika: %d\n", temp->ime, temp->brStanovnika);
		}
		temp = temp->next;
	}
	return 0;
}

int ispisiStablo(position current)
{
	if(!current)
		return 0;

	ispisiStablo(current->left);
	printf("\n%s\n", current->ime);
	ispisiListu(current->head);
	ispisiStablo(current->right);

	return 0;
}

int ispisiListu(positionNode head)
{
	positionNode temp = head->next;
	while (temp)
	{
		printf("%s %d\n", temp->ime, temp->brStanovnika);
		temp = temp->next;
	}
	return 0;
}
