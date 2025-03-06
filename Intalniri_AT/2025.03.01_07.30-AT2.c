#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

struct Client {
	int id;
	int varsta;
	char* nume;
	float buget;
	char initialaTata;
};
struct Client initializare(int id, int varsta, const char* nume, float buget, char initiala) {
	struct Client c;
	c.id = id;
	c.varsta = varsta;
	c.nume = (char*)malloc((strlen(nume) + 1) * sizeof(char));
	strcpy_s(c.nume, strlen(nume) + 1, nume);
	c.buget = buget;
	c.initialaTata = initiala;
	return c;
}

void afisare(struct Client c) {
	printf("ID: %d, varsta: %d\n", c.id, c.varsta);
	printf("Nume: %s\n", c.nume);
	printf("Buget: %.2f\n", c.buget);
	printf("Initiala tata: %c\n", c.initialaTata);
}

void afisareVector(struct Client* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
		printf("\n");
	}
}

float calculVarstaMedie(struct Client* clienti, int nrElemente) {
	float suma = 0;
	for (int i = 0; i < nrElemente; i++) {
		suma += clienti[i].varsta;
	}
	return suma / nrElemente;
}

struct Client* copiazaPrimeleNElemente(struct Client* vector, int nrElemente, int nrElementeCopiate) {
	struct Client* v = NULL;
	if (nrElementeCopiate <= nrElemente) {
		v = (struct Client*)malloc(sizeof(struct Client) * nrElementeCopiate);
		for (int i = 0; i < nrElementeCopiate; i++) {
			v[i] = vector[i];
			v[i].nume = (char*)malloc(strlen(vector[i].nume) + 1);
			strcpy_s(v[i].nume, strlen(vector[i].nume) + 1, vector[i].nume);
		}
	}
	return v;
}

void dezalocare(struct Client** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++) {
		free((*vector)[i].nume);
	}
	free(*vector);
	*nrElemente = 0;
	*vector = NULL;
}

void copiazaClientiCuBugetMare(struct Client* vector, char nrElemente, float bugetMinim, struct Client** vectorNou, int* dimensiune) {
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].buget >= bugetMinim) {
			(*dimensiune)++;
		}
	}
	*vectorNou = (struct Client*)malloc(sizeof(struct Client) * (*dimensiune));
	int k = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].buget >= bugetMinim) {
			(*vectorNou)[k] = vector[i];
			(*vectorNou)[k].nume = (char*)malloc(strlen(vector[i].nume) + 1);
			strcpy_s((*vectorNou)[k].nume, strlen(vector[i].nume) + 1, vector[i].nume);
			k++;
		}
	}
}

struct Client getPrimulClientDupaNume(struct Client* vector, int nrElemente, const char* numeCautat) {
	struct Client c;
	for (int i = 0; i < nrElemente; i++) {
		if (strcmp(vector[i].nume, numeCautat) == 0) {
			c = vector[i];
			c.nume = (char*)malloc(strlen(vector[i].nume) + 1);
			strcpy_s(c.nume, strlen(vector[i].nume) + 1, vector[i].nume);
			return c;
		}
	}

}

void inserareInVector(struct Client** vector, int* dim, struct Client client) {
	struct Client* aux = (struct Client*)malloc(sizeof(struct Client) * ((*dim) + 1));
	for (int i = 0; i < *dim; i++) {
		aux[i] = (*vector)[i];
	}
	aux[(*dim)] = client;
	aux[*dim].nume = (char*)malloc(strlen(client.nume) + 1);
	strcpy_s(aux[*dim].nume, strlen(client.nume) + 1, client.nume);
	free(*vector);
	*vector = aux;
	(*dim)++;
}

struct Client citireClientFisier(FILE* f) {
	struct Client c;
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, f);
	c.id = atoi(strtok(buffer, sep));
	c.varsta = atoi(strtok(NULL, sep)); //NULL continua citirea de la ultimul apel
	char* aux;
	aux = strtok(NULL, sep);
	c.nume = (char*)malloc(strlen(aux) + 1);
	strcpy_s(c.nume, strlen(aux) + 1, aux);
	c.buget = atoi(strtok(NULL, sep));
	c.initialaTata = strtok(NULL, sep)[0];
	return c;
}

struct Client* citireVectorDinFisier(const char* numeFisier, int* dim) {
	FILE* f = fopen(numeFisier, "r");
	struct Client* vector = NULL;
	(*dim) = 0;
	while (!feof(f)) {
		struct Client c = citireClientFisier(f);
		inserareInVector(&vector, dim, c);
	}
	fclose(f);
	return vector;
}

struct Nod {
	struct Client client;
	struct Nod* next;
};

void inserareListaSfarsit(struct Nod** cap, struct Client c) {
	struct Nod* nou = (struct Nod*)malloc(sizeof(struct Nod));
	nou->client = c; //(*nou).client = c
	nou->client.nume = (char*)malloc(strlen(c.nume) + 1);
	strcpy_s(nou->client.nume, strlen(c.nume) + 1, c.nume);
	nou->next = NULL;

	if (*cap) {
		struct Nod* p = *cap;
		while (p->next != NULL) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}

struct Nod* citireListaDinFisier(const char* numeFisier) {
	struct Nod* cap = NULL;
	FILE* f = open(numeFisier, "r");
	if (f != NULL) {
		while (!feof(f)) {
			struct Client c = citireClientFisier(f);
			inserareListaSfarsit(&cap, c);
			free(c.nume);
		}
	}
	fclose(f);
	return cap;
}

void afiseazaLista(struct Nod* cap) {
	while (cap) {
		afisare(cap->client);
		cap = cap->next;
	}
}

void stergereLista(struct Nod** cap) {
	while ((*cap) != NULL) {
		struct Nod* aux = *cap;
		(*cap) = (*cap)->next;
		free(aux->client.nume);
		free(aux);
	}
}

int main() {

	/*
	struct Client client = initializare(2, 23, "Popescu", 2000, 'B');
	afisare(client);

	//struct Client multiClienti[30]; //ALOCARE STATICA
	int nrClienti = 3;
	struct Client* clienti;
	clienti = (struct Client*)malloc(nrClienti * sizeof(struct Client)); //ALOCARE DINAMICA
	clienti[0] = initializare(1, 22, "Ionescu", 100, 'J');
	clienti[1] = initializare(2, 23, "Vasilescu", 200, 'D');
	clienti[2] = initializare(3, 23, "Gigel", 400, 'T');

	printf("Clientul cautat:\n");
	struct Client c = getPrimulClientDupaNume(clienti, nrClienti, "Gigel");
	afisare(c);

	inserareInVector(&clienti, &nrClienti, c);
	printf("\n\nAfisare vector actualizat:\n");
	afisareVector(clienti, nrClienti);

	free(c.nume);
	printf("Vector:\n");
	afisareVector(clienti, nrClienti);
	printf("%.2f\n", calculVarstaMedie(clienti, nrClienti));

	int nrClientiFideli = 2;
	struct Client* vectorScurt = copiazaPrimeleNElemente(clienti, nrClienti, nrClientiFideli);

	printf("\n\nClientiFideli:\n");
	afisareVector(vectorScurt, nrClientiFideli);


	struct Client* clientiCuBugetMare = NULL;
	int nrClientiCuBugetMare = 0;

	copiazaClientiCuBugetMare(clienti, nrClienti, 300, &clientiCuBugetMare, &nrClientiCuBugetMare);
	printf("\nClienti cu buget mare:\n");
	afisareVector(clientiCuBugetMare, nrClientiCuBugetMare);

	dezalocare(&clienti, &nrClienti);

	printf("Vectorul dupa stergere:\n");
	afisareVector(clienti, nrClienti);
	*/

	//FILE* f = fopen("clienti.txt", "r");
	//struct Client c = citireClientFisier(f);
	//afisare(c);

	//struct Client* clienti = NULL;
	//int nrClienti = 0;
	//clienti = citireVectorDinFisier("clienti.txt", &nrClienti);
	//afisareVector(clienti, nrClienti);

	struct Nod* cap = NULL;
	cap = citireListaDinFisier("clienti.txt");

	afiseazaLista(cap);
	stergereLista(&cap);
	printf("Dupa stergere:\n");
	afiseazaLista(cap);

	return 0;
}