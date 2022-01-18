#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Stablo;
typedef struct Stablo* position;
typedef struct Stablo
{
	char ime[128];
	int brStanovnika;
	position left;
	position right;
} stablo;

struct Node;
typedef struct Node* positionNode;
typedef struct Node
{
	char ime[128];
	positionNode next;
	position root; //svaki cvor/element liste sadrzi stablo 
} node;

position citajStablo(char* ime, position root);
int Sortiraj(positionNode novi, positionNode head);
int InsertAfter(positionNode prev, positionNode novi);
position unesiStablo(position novi, position current);
position noviElementStablo(char* ime, int broj);
position pronadiGrad(position current, int broj);
int ispisiStablo(position current);
int ispisiListu(positionNode current);

//funkcije za hash tablicu
int citajDatoteku(char* filename, positionNode head, node tablica[]); 
int pronadiDrzavu(node tablica[], char* drzava, int broj); 
int hashTablica(node tablica[], positionNode novi); //popunjava tablicu imenima drzava
int Lista(positionNode head); //inicijaliziramo vezanu listu odredenog indeksa (sve na nulu)
int Kljuc(char* drzava); //vraca indeks tablice, mjesto u nizu na kojem se nalazi odredena drzava

int main()
{
	node head;
	node tablica[11];
	int broj = 0;
	int i;
	char ime[124] = {0};
	head.ime = {0};
	head.next = NULL;
	head.root = NULL;

	for(i = 0; i < 11; i++)
	{
		Lista(&tablica[i]); //prvo sve liste u tablici inicijalizirat na nulu
	}

	citajDatoteku("drzave.txt", &head, tablica); //zatim popunjavat vrijednosti tablice/listi

	printf("Unesite drzavu u kojoj se nalazi grad kojeg zelite pretraziti i zeljeni broj stanovnika.\n");
	scanf(" %s", ime);
	scanf("%d", &broj);

	pronadiDrzavu(&head, ime, broj);

	return 0;
}

int pronadiDrzavu(node tablica[], char* drzava, int broj)
{
	int key = Kljuc(drzava); 

	positionNode temp = (tablica + key); //adresa drzave koja se u tablici nalazi na tablica[key] mjestu
	while(temp != NULL && Kljuc(temp->ime) != key) //usporedujemo kljuceve
		temp = temp->next;

	if(temp)
	{
		printf("Gradovi koji imaju veci broj stanovnika:\n");
		temp->root = pronadiGrad(temp->root, broj);
	}
	else
		printf("Navedena drzava ne postoji u datoteci!\n");

	return 0;
}

int citajDatoteku(char* filename, positionNode head, node tablica[])
{
	char buffer[1024] = {0};
	char ime[128] = {0}; //za ime datoteke u datoteci(file za gradove)
	FILE* dat = NULL;

	dat = fopen(filename, "r");
	if(dat == NULL)
	{
		perror("Datoteka nije otvorena!!\n");
		return -1;
	}
	
	while(!feof(dat))
	{
		positionNode novi = NULL;
		novi = (positionNode)malloc(sizeof(node)); //prvo formiramo listu sa nazivima drzava 
		if(novi == NULL)
		{
			perror("Neuspjela alokacija memorije!!\n");
			return -2;
		}

		fgets(buffer, 1024, dat);

		if (sscanf(buffer, " %s  %s", novi->ime, ime) == 2) //kada procitamo oba podatka
		{
			novi->root = NULL;
			novi->root = citajStablo(ime, novi->root); //formiramo stablo na taj root
			hashTablica(tablica, novi); //postavljamo novi cvor na odredeno mjesto tablice
		}
	}
	fclose(dat);
	return 0;
}

int hashTablica(node tablica[], positionNode novi)
{
	int key = 0;

	key = Kljuc(novi->ime);

	Sortiraj(novi, &tablica[key]);

	return 0;
}

int Kljuc(char* drzava)
{
	int key = 0;
	int br = 0;

	for(br = 0; br < 5;br++)
	{
		drzava[br] += key; //zbrojimo prvih pet slova(ASCII vrijednosti) imena drzava
	}
	return key % 11; //podijelimo kljuc sa velicinom tablice da dobijemo indeks(u tablici) na koji cemo postaviti taj kljuc
}

int Lista(positionNode head)
{
	memset(head->ime, 0, 128); //ovu funkciju korisitimo da bi popunili odredeni blok memorije....memset(*x, y, z)
	head->root = NULL;                                                                   // *x - pocetna adresa odakle krecemo punit
	head->next = NULL;                                                                   // y - vrijednost koju pohranjujemo
	                                                                                     // z - broj bajtova koje cemo popunit
	return 0;

}

position citajStablo(char* ime, position root)
{
	char buffer[1024] = {0};
	FILE* dat = NULL;
	
	dat = fopen(ime, "r");
	if(dat == NULL)
	{
		perror("Datoteka nije uspjesno otvorena!!\n");
		return NULL;
	}

	while(!feof(dat))
	{
		int broj = 0; //za broj stanovnika iz filea sa gradovima
		char grad[128] = {0}; //za ime grada iz filea sa gradovima
		position novi = NULL;

		fgets(buffer, 1024, dat);
		if (sscanf(buffer, " %s %d", grad, &broj) == 2)
		{
			novi = noviElementStablo(grad, broj);
			root = unesiStablo(novi, root);
		}
	}

	fclose(dat);
	return root;
}

position unesiStablo(position novi, position current)
{
	if(current == NULL)
		return novi;
	else if (current->brStanovnika < novi->brStanovnika)
		current->right = unesiStablo(novi, current->right);
	else if (current->brStanovnika > novi->brStanovnika)
		current->left = unesiStablo(novi, current->left);
	else 
	{
		if(strcmp(current->ime, novi->ime) < 0)
			current->right = unesiStablo(novi, current->right);

		else if (strcmp(current->ime, novi->ime) > 0)
			current->left = unesiStablo(novi, current->left);
		else //ako je rijec o istom gradu
			free(novi);
	}

	return current;
}

position noviElementStablo(char* ime, int broj)
{
	position novi = NULL;

	novi = (position)malloc(sizeof(stablo));

	if(!novi)
	{
		perror("Memorija nije uspjesno alocirana!!\n");
		return NULL;
	}

	strcpy(novi->ime, ime);
	novi->brStanovnika = broj;
	novi->left = NULL;
	novi->right = NULL;

	return novi;
}

int Sortiraj(positionNode novi, positionNode head)
{
	positionNode temp = head;

	while(temp->next != NULL && strcmp(temp->next->ime, novi->ime) < 0)
		temp = temp->next;

	InsertAfter(temp, novi);
	
	return 0;
}

int InsertAfter(positionNode prev, positionNode novi)
{
	novi->next = prev->next;
	prev->next = novi;

	return 0;
}

position pronadiGrad(position current, int broj)
{
	if(current == NULL) //nije upisan niti jedan grad u stablo
		return NULL;

	else if (current->brStanovnika >= broj)
	{
		current->left = pronadiGrad(current->left, broj);
		printf("Grad: %s  Broj stanovnika: %d \n", current->ime, current->brStanovnika);
		current->right = pronadiGrad(current->right, broj);
	}
	else if (current->brStanovnika < broj)
		current->right = pronadiGrad(current->right, broj); //desno podstablo,veci brStanovnika od roditelja

	return current;
}

int ispisiListu(positionNode current)
{
	while(current)
	{
		printf("Drzava: %s	Gradovi: \n", current->ime);
		if(current->root)
			ispisiStablo(current->root);
		else
			printf("Nisu uneseni gradovi!\n");

		current = current->next;
	}
	return 0;
}

int ispisiStablo(position current)
{
	if(!current)
		return 0;

	ispisiStablo(current->left);
	printf("%s %d\n", current->ime, current->brStanovnika);
	ispisiStablo(current->right);

	return 0;
}
