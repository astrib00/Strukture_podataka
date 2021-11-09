#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 1024

typedef struct Polinom* pozicija;

typedef struct Polinom
{
	int baza;
	int eksponent;
	pozicija ppol;
} polinom;

int CitajDatoteku(pozicija,pozicija);
int Ispis(pozicija);
int UcitajPolinom(pozicija, char*);
int ZbrojiPolinome(pozicija, pozicija, pozicija);
int MnoziPolinome(pozicija, pozicija, pozicija);
pozicija NoviElement(int, int);
int SortiraniUnos(pozicija, pozicija);

int main()
{
	polinom head1, head2, headZ, headM;
	char znak;
	head1.ppol = NULL;
	head2.ppol = NULL;
	headZ.ppol = NULL;
	headM.ppol = NULL;

	CitajDatoteku(&head1, &head2);

	printf("Izbornik - \nZ - zbrajanje polinoma\nM - mnozenje polinoma\n \n");
	scanf(" %c", &znak);

	switch(toupper(znak))
	{
		case 'Z':
			ZbrojiPolinome(&head1, &head2, &headZ);
			printf("Rezultat zbrajanja polinoma: \n");
			Ispis(headZ.ppol);
			break;
		case 'M':
			MnoziPolinome(head1.ppol, head2.ppol, &headM);
			printf("Rezultat mnozenja polinoma: \n");
			Ispis(headM.ppol);
			break;
		default:
			printf("Unijeli ste neispravnu komandu!! GRESKA!!!\n");
			break;
	}
	
	return 0;
}

int CitajDatoteku(pozicija Pos1, pozicija Pos2)
{
	FILE* datoteka = NULL;
	char buffer[MAX_SIZE] = { NULL };
	int status = 0;

	datoteka = fopen("polinomi.txt", "r");
	if(datoteka == NULL)
	{
		perror("Datoteka nije uspjesno otvorena!!\n");
		return -1;
	}

	fgets(buffer, MAX_SIZE, datoteka);
	status = UcitajPolinom(Pos1, buffer);

	fgets(buffer, MAX_SIZE, datoteka);
	status = UcitajPolinom(Pos2, buffer);

	fclose(datoteka);

	return 0;
}

int UcitajPolinom(pozicija Pos, char* buffer)
{
	char* currentBuffer = buffer;
	int baza = 0;
	int eksponent = 0;
	int pomak = 0;
	int status = 0;
	pozicija noviEl = NULL;

	while(strlen(currentBuffer) > 0)
	{
		status = sscanf(currentBuffer, " %d %d %n", &baza, &eksponent, &pomak);
		if(status != 2)
		{
			printf("Datoteka nije dobra!\n");
			return -1;
		}

		noviEl = NoviElement(baza, eksponent);
		if(noviEl == NULL)
		{
			return -1;
		}

		SortiraniUnos(Pos, noviEl);

		currentBuffer += pomak;
	}


	return 0;
}

int SortiraniUnos(pozicija Pos, pozicija noviEl)
{
	pozicija temp = Pos;

	while(temp->ppol != NULL && temp->ppol->eksponent < noviEl->eksponent)
	{
		temp = temp->ppol;
	}

	if(Pos->ppol == NULL || Pos->ppol->eksponent != noviEl->eksponent)
	{
		noviEl->ppol = Pos->ppol;
		Pos->ppol = noviEl;
	}

	return 0;
}

pozicija NoviElement(int baza, int eksponent) //funkcija u kojoj kreiramo novi element na temelju podataka ucitanih iz filea
{
	pozicija element = NULL;

	element = (pozicija)malloc(sizeof(polinom));

	if(element == NULL)
	{
		perror("Memorija nije ispravno alocirana!!\n");
		return element; //nemozemo vratit -1 kao signal za gresku jer je funkcija tipa pozicija pa vracamo NULL(element)
	}

	element->baza = baza;
	element->eksponent = eksponent;
	element->ppol = NULL;

	return element;
}

int Ispis(pozicija Pos)
{
	while(Pos != NULL)
	{
		printf("Baza je: %d    Eksponent je: %d \n", Pos->baza, Pos->eksponent);
		Pos = Pos->ppol;
	}
	return 0;
}

int ZbrojiPolinome(pozicija Pos1, pozicija Pos2, pozicija Rez)
{
	pozicija temp; //trebat ce nam za slucaj kada dodemo do kraja jedne od dvije liste

	while(Pos1->ppol != NULL && Pos2->ppol != NULL) //setamo kroz obje liste(polinoma), petlja se prekida cim u jednoj listi dodemo do kraja
	{
		pozicija novi = NULL;
		novi = (pozicija)malloc(sizeof(polinom));
		if(novi == NULL)
		{
			perror("Neuspjesna alokacija memorije!!!\n");
			return -1;
		}

		novi->ppol = NULL;

		if(Pos1->ppol->eksponent < Pos2->ppol->eksponent) //koristimo ovakav zapis jer kao argument saljemo &head...Kada bi slali head.next mogli bi pisati Pos2->eksponent itd...
		{
			novi->eksponent = Pos1->ppol->eksponent;
			novi->baza = Pos1->ppol->baza;
			Pos1 = Pos1->ppol;
		}
		else if(Pos1->ppol->eksponent > Pos2->ppol->eksponent)
		{
			novi->eksponent = Pos2->ppol->eksponent;
			novi->baza = Pos2->ppol->baza;
			Pos2 = Pos2->ppol;
		}
		else //ukoliko su im eksponenti jednaki
		{
			novi->eksponent = Pos1->ppol->eksponent;
			novi->baza = (Pos1->ppol->baza + Pos1->ppol->baza); //zbrojimo baze tih dvaju polinoma i spremimo u novi polinom
			Pos1 = Pos1->ppol;
			Pos2 = Pos2->ppol;
		}

		Rez->ppol = novi; //postavljanje pokazivaca u listi koja sadrzi rezultat
		Rez = novi;
	}
	
	if(Pos1->ppol == NULL) //ako smo u jednoj od lista dosli do kraja onda se izvrsava ovaj dio funkcije
		temp = Pos2->ppol;
	else
		temp = Pos1->ppol;
	while(temp != NULL)
	{
		pozicija novi = NULL;
		novi = (pozicija)malloc(sizeof(polinom));
		if(novi == NULL)
		{
			perror("Neuspjesna alokacija memorije!!!\n");
			return -1;
		}

		novi->ppol = NULL;
		novi->eksponent = temp->eksponent;
		novi->baza = temp->baza;
		Rez->ppol = novi;
		Rez = novi;
		temp = temp->ppol;
	}

	return 0;
}

int MnoziPolinome(pozicija Pos1, pozicija Pos2, pozicija Rez) //za argumente saljemo head.ppol
{
	polinom Pol; //treba nam za spremanje dobivenih vrijednosti prilikom prolazaka kroz while petlje

	pozicija novi = NULL;
		novi = (pozicija)malloc(sizeof(polinom));
		if(novi == NULL)
		{
			perror("Neuspjesna alokacija memorije!!!\n");
			return -1;
		}
		novi->ppol = NULL;
		Pol.ppol = novi; //dodjeljujemo mu vrijednost 'novi' u koju ce se kasnije pohranjivat vrijednosti dobivene zbrajanjem/mnozenjem eksponenata/baza

		while(Pos1 != NULL) //za svaki element prve liste
		{
			while(Pos2 != NULL)//prode kroz cijelu drugu listu i mnozi taj iz prve sa svakim iz druge
			{
				novi->eksponent = Pos1->eksponent + Pos2->eksponent;
				novi->baza = Pos1->baza * Pos2->baza;

				ZbrojiPolinome(&Pol, Rez, Rez);
				Pos2 = Pos2->ppol;
			}

			Pos1 = Pos1->ppol;
		}
		
		return 0;
}
