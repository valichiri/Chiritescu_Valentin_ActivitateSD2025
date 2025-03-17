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

struct Nod {
	Masina info;
	struct Nod* prev;
	struct Nod* next;
};
typedef struct Nod Nod;

struct ListaDubla {
	Nod* prim;
	Nod* ultim;
};
typedef struct ListaDubla LD;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	char* strtokNextPointer = NULL;
	aux = strtok_s(buffer, sep, &strtokNextPointer);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok_s(NULL, sep, &strtokNextPointer));
	m1.pret = atof(strtok_s(NULL, sep, &strtokNextPointer));
	aux = strtok_s(NULL, sep, &strtokNextPointer);
	m1.model = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok_s(NULL, sep, &strtokNextPointer);
	m1.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok_s(NULL, sep, &strtokNextPointer);
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

void afisareListaMasiniDeLaInceput(LD lista) { //prin valoare
	Nod* p = lista.prim;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}
void afisareListaMasiniDeLaSfarsit(LD lista) {
	Nod* p = lista.ultim;
	while (p) {
		afisareMasina(p->info);
		p = p->prev;
	}
}

void adaugaMasinaInLista(LD* lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua; //shallow copy
	nou->prev = NULL;
	nou->next = NULL;
	if (lista->prim != NULL) {
		nou->prev = lista->ultim;
		lista->ultim->next = nou;
	}
	else {
		lista->prim = nou;
	}
	lista->ultim = nou;
}

//void adaugaLaInceputInLista(/*lista dubla de masini*/ Masina masinaNoua) {
//	//adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru
//}

LD citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	LD lista;
	lista.prim = NULL;
	lista.ultim = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInLista(&lista, m);
	}
	fclose(f);
	return lista; //prin valoare
}

void dezalocareLDMasini(LD* lista) {
	if (lista->ultim == NULL && lista->prim == NULL) {
		return;
	}
	while (lista->prim->next) {
		lista->prim = lista->prim->next;
		free(lista->prim->prev->info.numeSofer);
		free(lista->prim->prev->info.model);
		free(lista->prim->prev);
	}
	free(lista->ultim->info.numeSofer);
	free(lista->ultim->info.model);
	free(lista->ultim);
	lista->ultim = NULL;
	lista->prim = NULL;
}

float calculeazaPretMediu(LD lista) {
	float suma = .0f;
	int contor = 0;
	Nod* p = lista.prim;
	while (p) {
		suma += p->info.pret;
		contor++;
		p = p->next;
	}
	if (contor > 0) {
		return suma / contor;
	}
	return 0;
}

void stergeMasinaDupaID(LD* lista, int id) {
	Nod* p = lista->prim;
	while (p && p->info.id != id) {
		p = p->next;
	}
	if (p == NULL) {
		printf("ID-ul %d nu a fost gasit in lista, nicio masina nu a fost stearsa.\n\n");
		return;
	}
	if (p->prev == NULL && p->next == NULL) {
		lista->prim = NULL;
		lista->ultim = NULL;
		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
		printf("Masina cu ID-ul %d a fost stearsa din lista, acum lista este goala.\n\n", id);
		return;
	}
	if (p->prev == NULL) {
		lista->prim = p->next;
		lista->prim->prev = NULL;
		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
		printf("Masina cu ID-ul %d a fost stearsa din lista.\n\n", id);
		return;
	}
	if (p->next == NULL) {
		lista->ultim = p->prev;
		lista->ultim->next = NULL;
		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
		printf("Masina cu ID-ul %d a fost stearsa din lista.\n\n", id);
		return;
	}
	if (p->prev != NULL && p->next != NULL) {
		p->prev->next = p->next;
		p->next->prev = p->prev;
		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
		printf("Masina cu ID-ul %d a fost stearsa din lista.\n\n", id);
		return;
	}
	
}

char* getNumeSoferMasinaScumpa(/*lista dublu inlantuita*/) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	return NULL;
}

int main() {

	LD lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaInceput(lista);
	printf("\n\nInvers:\n\n");
	afisareListaMasiniDeLaSfarsit(lista);
	
	printf("Pretul mediu este: %.2f\n\n",calculeazaPretMediu(lista));
	
	stergeMasinaDupaID(&lista, 111);
	stergeMasinaDupaID(&lista, 1);
	stergeMasinaDupaID(&lista, 10);
	stergeMasinaDupaID(&lista, 5);
	afisareListaMasiniDeLaInceput(lista);

	stergeMasinaDupaID(&lista, 2);
	stergeMasinaDupaID(&lista, 3);
	stergeMasinaDupaID(&lista, 4);
	stergeMasinaDupaID(&lista, 6);
	stergeMasinaDupaID(&lista, 7);
	stergeMasinaDupaID(&lista, 8);
	stergeMasinaDupaID(&lista, 9);
	afisareListaMasiniDeLaInceput(lista);

	dezalocareLDMasini(&lista);
	return 0;
}