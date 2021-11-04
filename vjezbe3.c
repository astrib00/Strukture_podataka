#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct _Osoba* pozicija; //pokazivac na strukturu _Osoba

typedef struct _Osoba
{
	char ime[128];
	char prezime[128];
	int godina;
	pozicija next;
} osoba;

pozicija NovaOsoba();
int UnesiPocetak (pozicija);
int IspisiListu (pozicija);
pozicija PronadiZadnji (pozicija);
int UnesiKraj(pozicija);
pozicija PronadiOsobu(pozicija);
pozicija TraziPrethodni(pozicija, char prezime[128]);
int Brisi(pozicija);
int UnesiIza(pozicija);
int UnesiIspred(pozicija);
int SortirajListu(pozicija);
int UpisiDatoteku(pozicija);
int CitajDatoteku(pozicija);

int main (int argc, char** argv)
{
	int number = 0;
	char znak = NULL;
	osoba head;
	head.next = NULL;
	

	printf("Pozdrav! Za zeljenu naredbu morate unijeti odgovarajuci broj. \nU nastavku je uputa koji broj treba za koju naredbu. \n");
	printf("Unos na pocetak liste - 1		Unos na kraj liste - 2 \nIspisivanje liste - 3		Trazenje odredene osobe u listi - 4\n");
	printf("Brisanje odredene osobe iz liste - 5		Unos iza odredene osobe - 6\nUnos ispred odredene osobe - 7		Sortiranje liste po prezimenima - 8\n");
	printf("Upisivanje liste u datoteku - 9		Ucitavanje liste iz datoteke - 10\n");

	while(toupper(znak) != 'P') //korisnik pritiskom na tipku P izlazi iz petlje
	{
		printf("\nUnesite zeljeni broj!\n");
			scanf("%d", &number);
	
	switch(number)
	{
		case 1:
			UnesiPocetak(&head);
			break;
		case 2:
			UnesiKraj(&head);
			break;
		case 3:
			IspisiListu(head.next);
			break;
		case 4:
			PronadiOsobu(&head);
			break;
		case 5:
			Brisi(&head);
			break;
		case 6:
			UnesiIza(&head);
			break;
		case 7:
			UnesiIspred(&head);
			break;
		case 8:
			SortirajListu(&head);
			break;
		case 9:
			UpisiDatoteku(head.next);
			break;
		case 10:
			CitajDatoteku(&head);
			break;
		default: 
			printf("GRESKA!! Niste unijeli ispravnu komandu!!\n");
			break;
	}

	printf("Ako zelite izaci iz menua pritisnite P, u protivnom pritisnite C!\n");
		scanf(" %c", &znak);
	}
	
	return 0;
}

pozicija NovaOsoba()
{
	pozicija temp;

	temp = (pozicija)malloc(sizeof(osoba)); //alociramo prostor za novi cvor(strukturu)

	if(temp == NULL) 
	{
		perror("GRESKA!!! Memorija nije alocirana.");
	}

	printf("Unesite podatke za novu osobu.\n");
	scanf("%s %s %d", temp->ime, temp->prezime, &temp->godina);

	return temp;
}

int UnesiPocetak (pozicija Pos) //funkcija unosi element na pocetak liste
{
	pozicija temp;

	temp = NovaOsoba();
	
	temp->next = Pos->next;
	Pos->next = temp;

	return 0;
}

int IspisiListu (pozicija Pos) //funkcija ispisuje cijelu listu
{
	printf("Ispis liste: \n");

	while(Pos != NULL)
	{
		printf(" %s  %s  %d\n", Pos->ime, Pos->prezime, Pos->godina);
		Pos = Pos->next;
	}

	return 0;
}

pozicija PronadiZadnji (pozicija Pos) //trazimo zadnji element u listi
{
	while(Pos->next != NULL) //setamo do kraja liste
		Pos = Pos->next;

	return Pos; //vracamo poziciju zadnjeg elementa u listi
}

int UnesiKraj(pozicija Pos)
{
	pozicija temp;

	temp = NovaOsoba();

	Pos = PronadiZadnji(Pos);

	Pos->next = temp;
	temp->next = NULL; //jer unosimo na kraj liste

	return 0;
}

pozicija PronadiOsobu(pozicija Pos)
{
	char traziSe[128]; //prezime koje trazimo u listi

	printf("Unesite prezime osobe koju zelite pronaci u listi.\n");
	scanf("%s", traziSe);

	while(Pos->next != NULL && strcmp(traziSe, Pos->prezime) != 0) //setamo po listi dok ne nademo prezime ili dok ne dodemo do kraja liste
		Pos = Pos->next;

	if(strcmp(traziSe, Pos->prezime) == 0) //ako su stringovi jednaki strcmp vraca 0, u protivnom vraca 1
	{
		printf("Trazena osoba je: %s  %s  %d\n", Pos->ime, Pos->prezime, Pos->godina);
		return Pos;
	}
	else {
		printf("Osoba sa takvim prezimenom ne postoji!!!\n");
		return NULL;
	}
	
}

pozicija TraziPrethodni(pozicija Pos, char prezime[128]) //funkcija vraca element koji prethodi trazenom elementu
{
	pozicija prethodni;

	prethodni = (pozicija)malloc(sizeof(osoba)); //koristimo ga za pamcenje prethodnog elementa dok setamo listom

	while(Pos->next !=NULL && strcmp(prezime, Pos->prezime) != 0) //setamo po listi dok ne nademo prezime ili dok ne dodemo do kraja liste
	{
		prethodni = Pos; //u prethodni spremamo trenutni clan
		Pos = Pos->next; //a u trenutni spremamo onaj koji dolazi sljedeci po redu
	}

	if(strcmp(prezime, Pos->prezime) == 0)
		return prethodni; //vracamo prethodni clan
	else
		printf("Uneseno prezime ne postoji u listi!!!\n");
		return NULL;
}

int Brisi(pozicija Pos) //funkcija brise odredeni element iz liste
{
	char prezime[128]; //prezime koje zelimo ukloniti iz liste
	pozicija prethodni;

	printf("Unesite prezime osobe koju zelite ukloniti iz liste!\n");
	scanf("%s", prezime);

	prethodni = TraziPrethodni(Pos, prezime); //dodijelimo poziciju prethodnog clana
	Pos = prethodni->next; //dodijelimo poziciju trenutnog clana jer u funkciju TraziPrethodni saljemo Pos(&head) koja se izmjenjuje u toj funkciji, ali u ovoj ostaje na headu pa moramo i u ovoj funkciji postaviti Pos na ispravnu vrijednost da bi funkcija radila kako treba

	if(strcmp(prezime, Pos->prezime) == 0) //ako je prezime pronadeno
	{
		prethodni->next = Pos->next;
		free(Pos); //oslobodimo memoriju trazenog clana
	}else
		printf("Trazeno prezime ne postoji u listi!!!\n");

	return 0;
}

int UnesiIza(pozicija Pos)
{
	pozicija trazi; //varijabla u koju cemo spremiti trazenu osobu(onu izad koje vrsimo unos u listu)

	printf("Trazi se ona osoba u listi izad koje zelite upisati novu osobu!!\n");

	trazi = PronadiOsobu(Pos);

	if(trazi == NULL) //znaci da trazenog clana nema u listi!!
	{
		return -1;
	}

	UnesiPocetak(trazi); //kao argument saljemo onu adresu izad koje zelimo upisati novu osobu

	return 0;
}

int UnesiIspred(pozicija Pos)
{
	pozicija prethodni; //varijabla u koju spremamo adresu osobe koja se u listi nalazi mjesto ispred osobe koju trazimo
	char prezime[128];

	printf("Unesite prezime osobe iz liste ispred koje zelite upisati novu osobu!\n");
	scanf("%s", prezime);

	prethodni = TraziPrethodni(Pos, prezime);

	if(prethodni == NULL) //znaci da unesena osoba ne postoji u listi!!!
	{
		return -1;
	}

	UnesiPocetak(prethodni); //kao argument saljemo onu adresu izad koje zelimo upisati novu osobu 

	return 0;
}

int SortirajListu(pozicija Pos)
{
	pozicija prethodni, end, temp, i;

	end = NULL;

	while(Pos->next != end)
	{
		prethodni = Pos;
		i = Pos->next;

		while(i->next != end)
		{
			if(strcmp(i->prezime, i->next->prezime) > 0) //vrsimo usporedbu dva susjedna clana po prezimenu
			{
				temp = i->next;
				prethodni->next = temp;
				i->next = temp->next;
				temp->next = i;
				i = temp;
			}

			prethodni = i;
			i = i->next;

		}

		end = i;
	}
}

int UpisiDatoteku(pozicija Pos) //funkcija vrsi upisivanje elemenata liste u datoteku
{
	FILE* datoteka = NULL;

	datoteka = fopen("lista.txt", "w");

	if(datoteka == NULL)
	{
		perror("GRESKA!!!Datoteka nije uspjesno otvorena !! \n");
		return -1;
	}

	while(Pos != NULL) //da smo stavili Pos->next zadnji element nebi bia upisan u datoteku
	{
		fprintf(datoteka, " %s %s    %d\n", Pos->ime, Pos->prezime, Pos->godina);
		Pos = Pos->next;
	}

	printf("Podaci su uspjesno upisani u datoteku!!\n");
	fclose(datoteka);

	return 0;
}

int CitajDatoteku(pozicija Pos) //funkcija nam vrsi ucitavanje liste iz datoteke
{
	FILE* datoteka = NULL;
	pozicija novi;
	char buffer[1024]; //spremnik u kojeg cemo ucitati sadrzaj datoteke

	datoteka = fopen("lista.txt", "r");

	if(datoteka == NULL)
	{
		perror("GRESKA!!!Datoteka nije uspjesno otvorena !! \n");
		return -1;
	}
		
	while(!feof(datoteka))
	{
		
		fgets(buffer, 1024, datoteka);
		novi = (pozicija)malloc(sizeof(osoba));
		sscanf(buffer, "%s  %s     %d\n", novi->ime, novi->prezime, &novi->godina);

		novi->next = Pos->next;
		Pos->next = novi;
		Pos = novi;
	}
	
	
	fclose(datoteka); //obavezno na kraju rada s datotekom zatvoriti istu!!!

	return 0;
}
