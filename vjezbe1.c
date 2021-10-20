#include <stdio.h>
#include <stdlib.h>

typedef struct _student //novi tip podataka!
{
	char ime[128];
	char prezime[128];
	int br_bodova;
} student;

int IzbrojiStudente(FILE* datoteka) //funkcija ce vratiti broj studenata zapisanih u datoteci
{
	int broj = 0;

	while(!feof(datoteka))
	{
		
		if(fgetc(datoteka) == '\n') //da bi nam brojac bio tocan nakon zadnjeg studenta u datoteci trebamo pritisnuti enter
			broj++;
	}

	return broj;
}

void UpisiNiz(FILE* datoteka, student* niz)
{
	int n = 0;

	while(!feof(datoteka))
	{
		fscanf(datoteka, " %s       %s       %d", niz[n].ime, niz[n].prezime, &niz[n].br_bodova); //fscanf cita cijelu liniju(redak) u datoteci
		n++;
	}
}

void IspisiNiz(student* niz, int brStud, double pomocni[])
{
	int i = 0;

	for(i = 0; i < brStud; i++)
	{
		printf(" %s  %s  %d  %.2f \n", niz[i].ime, niz[i].prezime, niz[i].br_bodova, pomocni[i]);
	}

}

void RelativniBodovi(student* niz,int brStud, double pomocni[]) //koristimo pomocni niz da bi kasnije mogli ispisati i relativne bodove uz podatke studenta
{
	int i;
	double RelBod[128];
	int MaxBod = 80;

	for(i = 0; i < brStud; i++)
	{
		RelBod[i] = ((double)niz[i].br_bodova/(double)MaxBod * 100);
		pomocni[i] = RelBod[i]; //spremamo podatke u pomocni niz
	}

}

int main()
{
	int BrojStudenata = 0;
	FILE* fp = NULL;
	student* studenti;
	double RelativniBod[80];

	fp = fopen("popis.txt", "r");

		if (fp == NULL) //datoteka zatvorena
		{
			printf("GREŠKA! Datoteka nije uspješno otvorena! \n");
			return 1; 
		}

	BrojStudenata = IzbrojiStudente(fp);
	printf("Broj studenata na popisu je: %d \n\n", BrojStudenata);

	rewind(fp); //vracamo se na pocetak datoteke fp

	studenti = (student*)malloc(sizeof(student) * BrojStudenata); //dinamicki alociramo niz studenata

	UpisiNiz(fp, studenti); //ucitavamo podatke iz datoteke u dinamicki alociran niz

	RelativniBodovi(studenti, BrojStudenata, RelativniBod); //racunamo relativne bodove

	IspisiNiz(studenti, BrojStudenata, RelativniBod); //ispisujemo trazene podatke o studentu

	fclose(fp); //na kraju rada sa datotekom obavezno je zatvoriti!!!

	return 0;
}
