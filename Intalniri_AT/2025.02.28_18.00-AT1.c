#include<stdio.h>
#include<malloc.h>

struct Client {
	int id;
	int varsta;
	char* nume;
	float buget;
	char initialaTata;
};
struct Client initializare(int id, int varsta, const char* nume, float buget, char initialaTata) {
	struct Client c;
	c.id = 1;
	c.varsta = varsta;
	c.nume = (char*)malloc((strlen(nume) + 1) * sizeof(char));
	strcpy_s(c.nume, strlen(nume) + 1, nume);
	c.buget = buget;
	c.initialaTata = initialaTata;
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
	if (nrElementeCopiate < nrElemente) {
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



int main() {

	struct Client client = initializare(2, 23, "Popescu", 2000, 'B'); //"B"
	afisare(client);

	//struct Client multiClienti[30];

	int nrClienti = 3;
	struct Client* clienti;
	clienti = (struct Client*)malloc(nrClienti * sizeof(struct Client));
	clienti[0] = initializare(1, 22, "Ionescu", 100, 'J');
	clienti[1] = initializare(2, 23, "Vasilescu", 200, 'D');
	clienti[2] = initializare(3, 23, "Gigel", 400, 'T');

	printf("Clientul cautat:\n");
	struct Client c = getPrimulClientDupaNume(clienti, nrClienti, "Gigel");
	afisare(c);

	printf("\nVector:\n");
	afisareVector(clienti, nrClienti);
	printf("%.2f\n", calculVarstaMedie(clienti, nrClienti));

	int nrClientiFideli = 2;
	struct Client* vectorScurt = copiazaPrimeleNElemente(clienti, nrClienti, nrClientiFideli);
	printf("\n\nClienti Fideli:\n");
	afisareVector(vectorScurt, nrClientiFideli);

	struct Client* clientiCuBugetMare = NULL;
	int nrClientiCuBugetMare = 0;
	copiazaClientiCuBugetMare(clienti, nrClienti, 150, &clientiCuBugetMare, &nrClientiCuBugetMare);
	printf("Clienti cu buget mare:\n");
	afisareVector(clientiCuBugetMare, nrClientiCuBugetMare);

	dezalocare(&clienti, &nrClienti);
	printf("Vectorul dupa stergere:\n");
	afisareVector(clienti, nrClienti);



	return 0;
}