#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Osoba* pozicija; //pokazivac na strukturu _Osoba

typedef struct _Osoba
{
	char ime[128];
	char prezime[128];
	int godina;
	pozicija next;
} osoba;

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

int UpisiDatoteku(pozicija Pos)
{
	FILE* datoteka = NULL;

	datoteka = fopen("lista.txt", "w");

	if(datoteka == NULL)
	{
		perror("GRESKA!!!Datoteka nije uspjesno otvorena !! \n");
		return -1;
	}

	while(Pos != NULL)
	{
		fprintf(datoteka, "%s  %s    %d", Pos->ime, Pos->prezime, Pos->godina);
		Pos = Pos->next;
	}

	printf("Podaci su uspjesno upisani u datoteku!!\n");
	fclose(datoteka);

	return 0;
}

int CitajDatoteku() //funkcija nedovrsena, proucit jos malo sscanf i vidit kako to sve skupa slozit
{
	FILE* datoteka = NULL;
	pozicija novi;
	int i,br = 0;
	char* buffer = NULL;

	datoteka = fopen("lista.txt", "r");

	if(datoteka == NULL)
	{
		perror("GRESKA!!!Datoteka nije uspjesno otvorena !! \n");
		return -1;
	}

	while(!feof(datoteka))
	{
		fscanf(datoteka, "%s  %s      %d", buffer); //ucitava redak po redak
		br++; //brojac zbog toga da znamo koliko se puta izvrsila while petlja, treba nam taj podatak u nastavku
	}

	if(buffer == NULL)
	{
		perror("GRESKA!!! Podaci nisu uspjesno ucitani u buffer!\n");
		return -1;
	}

	rewind(datoteka);

	/*for(i = 0; i < br; i++)
	{


	}*/


}

int main (int argc, char** argv)
{
	osoba head;
	head.next = NULL;
	
	UnesiPocetak(&head);
	UnesiPocetak(&head);
	//UnesiPocetak(&head);
	//UnesiPocetak(&head);
	IspisiListu(head.next);
	UnesiKraj(&head);
	IspisiListu(head.next);
	PronadiOsobu(&head);
	Brisi(&head);
	IspisiListu(head.next);
	UnesiIza(&head);
	IspisiListu(head.next);
	UnesiIspred(&head);
	IspisiListu(head.next);
	SortirajListu(&head);
	IspisiListu(head.next);

	
	return 0;
}
