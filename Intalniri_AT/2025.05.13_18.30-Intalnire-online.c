#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-un arbore binar de cautare
struct Nod {
	Masina infoUtil;
	struct Nod* fiuStanga;
	struct Nod* fiuDreapta;
	//grad de echilibru
};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

int calculeazaInaltimeArbore(Nod* rad) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	if (rad) {
		int Hs = calculeazaInaltimeArbore(rad->fiuStanga);
		int Hd = calculeazaInaltimeArbore(rad->fiuDreapta);
		return 1 + (Hs > Hd ? Hs : Hd);
	}
	else {
		return 0;
	}
}

int calculGradEchilibru(Nod* rad) {
	return calculeazaInaltimeArbore(rad->fiuStanga) - calculeazaInaltimeArbore(rad->fiuDreapta);
}

void rotireStanga(Nod** rad) {
	Nod* aux = (*rad)->fiuDreapta;
	(*rad)->fiuDreapta = aux->fiuStanga;
	aux->fiuStanga = (*rad);
	(*rad) = aux;
}

void rotireDreapta(Nod** rad) {
	Nod* aux = (*rad)->fiuStanga;
	(*rad)->fiuStanga = aux->fiuDreapta;
	aux->fiuDreapta = (*rad);
	(*rad) = aux;
}

void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte princiippile de arbore binar de cautare
	//dupa o anumita cheie pe care o decideti - poate fi ID

	if ((*radacina) != NULL) {
		if (masinaNoua.id < (*radacina)->infoUtil.id) {
			adaugaMasinaInArbore(&((*radacina)->fiuStanga), masinaNoua);
		}
		else {
			adaugaMasinaInArbore(&((*radacina)->fiuDreapta), masinaNoua);
		}
		//verificam gradul de echilibru
		int GE = calculGradEchilibru(*radacina);
		//dezechilibru in dreapta
		if (GE == -2) {
			if (calculGradEchilibru((*radacina)->fiuDreapta) == 1) {
				//dubla rotire
				rotireDreapta(&(*radacina)->fiuDreapta);
			}
			rotireStanga(radacina); //&(*radacina)
		}
		//dezechilibru in stanga
		if (GE == 2) {
			if (calculGradEchilibru((*radacina)->fiuStanga) == 1) {
				//dubla rotire
				rotireStanga(&(*radacina)->fiuStanga);
			}
			rotireDreapta(radacina); //&(*radacina)
		}
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->fiuStanga = NULL;
		nou->fiuDreapta = NULL;
		nou->infoUtil = masinaNoua;
		nou->infoUtil.model = (char*)malloc(sizeof(char) * (strlen(masinaNoua.model) + 1));
		strcpy_s(nou->infoUtil.model, sizeof(char) * (strlen(masinaNoua.model) + 1), masinaNoua.model);
		nou->infoUtil.numeSofer = (char*)malloc(sizeof(char) * (strlen(masinaNoua.numeSofer) + 1));
		strcpy_s(nou->infoUtil.numeSofer, sizeof(char) * (strlen(masinaNoua.numeSofer) + 1), masinaNoua.numeSofer);
		(*radacina) = nou;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* file = fopen(numeFisier, "r");
	Nod* nou = NULL;
	if (file != NULL) {
		while (!feof(file)) {
			Masina m = citireMasinaDinFisier(file);
			adaugaMasinaInArbore(&nou, m);
			free(m.model);
			free(m.numeSofer);
		}
	}
	fclose(file);
	return nou;
}

void afisareArborePreordine(Nod* radacina) { //preordine RSD
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere

	if (radacina != NULL) {
		afisareMasina(radacina->infoUtil);
		afisareArborePreordine(radacina->fiuStanga);
		afisareArborePreordine(radacina->fiuDreapta);
	}
}

void afisareArboreInordine(Nod* radacina) { //inordine SRD
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere

	if (radacina != NULL) {
		afisareArboreInordine(radacina->fiuStanga);
		afisareMasina(radacina->infoUtil);
		afisareArboreInordine(radacina->fiuDreapta);
	}
}

void dezalocareArboreDeMasini(Nod** radacina) {
	//sunt dezalocate toate masinile si arborele de elemente

	//parcurgem postordine SDR
	if ((*radacina) != NULL) {
		dezalocareArboreDeMasini(&((*radacina)->fiuStanga));
		dezalocareArboreDeMasini(&((*radacina)->fiuDreapta));
		free((*radacina)->infoUtil.model);
		free((*radacina)->infoUtil.numeSofer);
		free(*radacina);
		(*radacina) = NULL;
	}
}

Masina getMasinaByID(Nod* rad, int id) {
	Masina m;
	m.id = -1;
	if (rad != NULL) {
		if (id < rad->infoUtil.id) {
			return getMasinaByID(rad->fiuStanga, id);
		}
		else if (id > rad->infoUtil.id) {
			return getMasinaByID(rad->fiuDreapta, id);
		}
		else {
			return rad->infoUtil;
		}
	}

	return m;
}

int determinaNumarNoduri(/*arborele de masini*/) {
	//calculeaza numarul total de noduri din arborele binar de cautare
	return 0;
}



float calculeazaPretTotal(/*arbore de masini*/) {
	//calculeaza pretul tuturor masinilor din arbore.
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(/*arbore de masini*/ const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	return 0;
}

int main() {
	Nod* radacina = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	printf("============ afisare ArboreInordine:\n");
	afisareArboreInordine(radacina);

	printf("============ Gasire id 6:\n");
	afisareMasina(getMasinaByID(radacina, 6));

	printf("============ afisare ArborePreordine:\n");
	afisareArborePreordine(radacina);

	return 0;
}