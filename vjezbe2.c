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

int PronadiOsobu(pozicija Pos)
{
	char traziSe[128]; //prezime koje trazimo u listi

	printf("Unesite prezime osobe koju zelite pronaci u listi.\n");
	scanf("%s", traziSe);

	while(Pos->next != NULL && strcmp(traziSe, Pos->prezime) != 0) //setamo po listi dok ne nademo prezime ili dok ne dodemo do kraja liste
		Pos = Pos->next;

	if(strcmp(traziSe, Pos->prezime) == 0) //ako su stringovi jednaki strcmp vraca 0, u protivnom vraca 1
		printf("Trazena osoba je: %s  %s  %d\n", Pos->ime, Pos->prezime, Pos->godina);
	else
		printf("Osoba sa takvim prezimenom ne postoji!!!\n");

	return 0;
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

	return prethodni; //vracamo prethodni clan
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
	
	return 0;
}
