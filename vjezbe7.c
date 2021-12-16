#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define velicina 256

typedef struct stablo* pozicija;

struct stablo
{
	char dir_name[velicina];
	pozicija parent;
	pozicija child1;
	pozicija child2; 
};

typedef struct stablo stab;

//funkcije za terminal
pozicija prijasnji_dir(pozicija p);
pozicija changeDir(pozicija argP, char* argIme);
pozicija provjera_naredbe(char* input, pozicija p);
pozicija odradi_naredbu(char* cmnd, char* argument, pozicija p);
//funkcije za stablo
pozicija make_dir(char* name);
int izbrisi(pozicija p);
int show(pozicija p);
int add_child1(pozicija p, char* name);
int add_child2(pozicija p, char* name);

int main()
{
	char input[velicina];
	char username[velicina];

	pozicija trDir = NULL; //postavimo da je trenutni direktorij prazan!

	stab root;
	root.child1 = NULL;
	root.child2 = NULL;
	root.parent = NULL;

	trDir = &root; //direktorij otvoren trenutno

	printf("Unesite ime za root direktorij.\n");
	gets(username);

	strcpy(root.dir_name, username);

	while(1)
	{
		printf("\n %s", trDir->dir_name); //ispisujemo ime trenutno otvorenog direktorija

		printf("Odradite unos: \n");
		gets(input);  //korisnikov unos

		trDir = provjera_naredbe(&input, trDir); //funkcija ce nam vratiti direktorij koji je trenutno otvoren

		if(trDir == NULL) //izac cemo iz programa ako nemamo nijedan otvoreni direktorij
			break;
	}

	izbrisi(root.child1);
	root.child1 = NULL;

	show(&root);

	return 0;
}

pozicija prijasnji_dir(pozicija p)
{
	if(p->parent == NULL)
	{
		printf("\nGreska!!!\n");
		return p;
	}
	else
		return p->parent;
}

pozicija changeDir(pozicija argP, char* argIme)
{
	pozicija p = argP->child1;

	if (*argIme == '\0' ) //pritisnut je samo enter
	{
		printf("Greska!! Ime direktorija nije uneseno!\n");
		return argP;
	}

	if(p == NULL)
	{
		printf("Direktorij u kojem se trenutno nalazite je prazan!\n");
		return argP;
	}

	else if(strcmp(p->dir_name, argIme) == 0) // ako je 0, direktorij je pronaden
	{
		return p;
	}

	while(p != NULL && strcmp(p->dir_name, argIme) != 0)
	{
		p = p->child2;
	}
}

pozicija provjera_naredbe(char* input, pozicija p)
{
	int read = 0;
	char* buffer;
	char command[velicina] = { 0 };
	char arg[velicina] = { 0 };

	buffer = (char*)calloc(strlen(input) + 20, sizeof(char)); //velicina unosa (input) plus jos 20 bitova za sigurnost

	if(buffer == NULL)
	{
		printf("\nGreska!! Memorija je nepravilno alocirana!!!\n"); // provjera buffera
		return -1;
	}

	strcpy(buffer, input); //prepisujemo u buffer ono sta je korisnik upisao

	printf("Ispis buffera: %s", buffer); //ispisujemo sadrzaj buffera

	read = sscanf(buffer, "%s %s", &command, &arg); //citamo iz buffera

	if(command == '\0')
		return p;

	if(read == -1)
	{
		printf("\n FATAL ERROR\n");
		return p;
	}

	if(read == 0)
	{
		printf("INPUT ERROR\n");
		return p;
	}

	else {
		return odradi_naredbu(&command, &arg, p);
	}
}

pozicija odradi_naredbu(char* cmnd, char* argument, pozicija p)
{
	if (strcmp(cmnd, "mkdir") == 0)
	{
		add_child1(p, argument);
	}

	else if(strcmp(cmnd, "cd") == 0)
	{
		return changeDir(p, argument);
	}

	else if(strcmp(cmnd, "cd..") == 0)
	{
		return prijasnji_dir(p);
	}

	else if (strcmp(cmnd, "ls") == 0)
	{
		show(p);
	}

	else if(strcmp(cmnd, "exit") == 0)
	{
		printf("Napustamo terminal.\n");
		return NULL;
	}

	else
		printf("GRESKA!! %s naredba ne postoji!\n", cmnd);

	return p;
}

pozicija make_dir(char* name)
{
	pozicija new_dir;

	new_dir = (pozicija)malloc(sizeof(stab));

	if(new_dir == NULL)
	{
		printf("Greska!!! Alokacija memorije za novi direktorij nije uspjela!\n");
		return NULL;
	}

	strcpy(new_dir->dir_name, name);
	new_dir->child1 = NULL;
	new_dir->child2 = NULL;

	printf("Novi direktorij imena: %s\n", name);
	return new_dir;
}

int izbrisi(pozicija p)
{
	if (p == NULL)
		return 0;

	izbrisi(p->child1);
	izbrisi(p->child2);

	printf("Obrisan je %s\n", p->dir_name);
	free(p);

	return 0;
}

int show(pozicija p) //funkcija koja pokazuje sadrzaj direktorija
{
	if(p == NULL)
	{
		printf("GRESKA!! Pointer sadrzi vrijednost NULL!\n");
		return -1;
	}

	if(p->child1 == NULL)
	{
		printf("%s direktorij je prazan!!\n", p->dir_name);
		return 0;
	}

	p = p->child1; //pokazivac postavljamo na prvo dijete

	while(p != NULL)
	{
		printf("%s \n", p->dir_name);
		p = p->child2; //prebacujemo pointer na drugo dijete
	}
	return 0;
}

int add_child1(pozicija p, char* name)
{
	if(p->child1 == NULL) //ako dijete ne postoji
	{
		p->child1 = make_dir(name);
		p->child1->parent = p;
	}

	else //ako vec postoji prvo dijete dodamo drugo dijete
	{
		add_child2(p, name);
	}

	return 0;
}

int add_child2(pozicija p, char* name) //za argument uzmemo roditelja
{
	pozicija parent = p;
	pozicija prev = NULL;

	pozicija new_child = make_dir(name);
	new_child->parent = parent;

	p = p->child1; //p postaje prvo dijete
	prev = p;

	if(strcmp(name, p->dir_name) < 0) //usporedujemo ime sa imenom prvog dijeteta
	{
		parent->child1 = new_child;
		new_child->child2 = p;
	}

	else
	{
		while(p->child2 != NULL && strcmp(name, p->dir_name) > 0)
		{
			prev = p;
			p = p->child2;
		}

		new_child->child2 = prev->child2;
		prev->child2 = new_child;
	}

	return 0;
}
