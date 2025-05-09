#include<stdio.h>
#include<stdlib.h>		//malloc
#include<stdbool.h>		//bool type
#include<string.h>

struct Fruct {
	char* denumire;
	float gramaj;		//32b (1-8-23)
	//double gramajD;	  64b (1-11-52)
	int nrZile;
	bool areSambure;
};

struct Fruct initializareFruct(const char* denumire, float gramaj, int nrZile, bool areSambure) {
	struct Fruct f;
	f.denumire = NULL;
	if (denumire != NULL) {
		f.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
		strcpy_s(f.denumire, sizeof(char) * (strlen(denumire) + 1), denumire);
	}
	f.gramaj = gramaj;
	f.nrZile = nrZile;
	f.areSambure = areSambure;
	return f;
}

void afisareFruct(struct Fruct fruct) {
	printf("%10s, %7.2f grame, %2d zile, %d sambure\n", fruct.denumire, fruct.gramaj, fruct.nrZile, fruct.areSambure);
}

void modificaDenumire(struct Fruct* f, const char* denumire) {
	if (f->denumire != NULL) {
		free(f->denumire);
		//(*f).denumire = NULL;
		f->denumire = NULL;
	}
	f->denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
	strcpy_s(f->denumire, sizeof(char) * (strlen(denumire) + 1), denumire);
}

void modificaNrZile(struct Fruct* f, int nrZile) {
	if (nrZile > 0) {
		f->nrZile = nrZile;
	}
}

void dezalocareFruct(struct Fruct* f) {
	if (f->denumire != NULL) {
		free(f->denumire);
		f->denumire = NULL;
	}
}

// VECTORI

void afisareVectorFructe(struct Fruct* fructe, int nrFructe) {
	printf("Afisare vector cu %d fructe:\n", nrFructe);
	for (int i = 0; i < nrFructe; i++) {
		printf("\t");
		afisareFruct(fructe[i]);
	}
	printf("\n");
}

struct Fruct* copiazaPrimeleNFructe(struct Fruct* vectorFructe, int nrFructe, int nrFructeDeCopiat) {
	struct Fruct* f = NULL;
	if (nrFructeDeCopiat <= nrFructe) {
		f = (struct Fruct*)malloc(sizeof(struct Fruct) * nrFructeDeCopiat);
		for (int i = 0; i < nrFructeDeCopiat; i++) {
			f[i] = initializareFruct(vectorFructe[i].denumire, vectorFructe[i].gramaj, vectorFructe[i].nrZile, vectorFructe[i].areSambure);
		}
	}
	return f;
}

void copiazaFructeVechi(struct Fruct* f, int nrFructe, int nrZileMin, struct Fruct** vectorNou, int* nrFructeVectorNou) {
	*nrFructeVectorNou = 0;
	for (int i = 0; i < nrFructe; i++) {
		if (f[i].nrZile >= nrZileMin) {
			(*nrFructeVectorNou)++;
		}
	}
	if ((*vectorNou) != NULL) {
		free(*vectorNou);
	}
	*vectorNou = (struct Fruct*)malloc(sizeof(struct Fruct) * (*nrFructeVectorNou));
	int k = 0;
	for (int i = 0; i < nrFructe; i++) {
		if (f[i].nrZile >= nrZileMin) {
			(*vectorNou)[k] = f[i];
			(*vectorNou)[k].denumire = (char*)malloc(sizeof(char) * (strlen(f[i].denumire) + 1));
			strcpy_s((*vectorNou)[k].denumire, sizeof(char) * (strlen(f[i].denumire) + 1), f[i].denumire);
			k++;
		}
	}
}

void dezalocareVectorFructe(struct Fruct** f, int* nrFructe) {
	if ((*nrFructe) > 0) {
		for (int i = 0; i < (*nrFructe); i++) {
			dezalocareFruct(&(*f)[i]);
		}
	}
	free(*f);
	*f = NULL;
	*nrFructe = 0;
}

int main() {
	struct Fruct* fructe = NULL;
	int nrFructe = 10;
	fructe = (struct Fruct*)malloc(sizeof(struct Fruct) * nrFructe);
	for (int i = 0; i < nrFructe; i++) {
		fructe[i] = initializareFruct("Mar", 120.5, 1, true);
		//*(fructe+i) = initializareFruct("Mar", 120.5, 1, true);
	}
	afisareVectorFructe(fructe, nrFructe);

	modificaDenumire(&fructe[2], "Portocala");
	modificaNrZile(&fructe[3], 15);
	dezalocareFruct(&fructe[4]);
	afisareVectorFructe(fructe, nrFructe);

	struct Fruct* copieFructe = NULL;
	int nrCopieFructe = 6;
	copieFructe = copiazaPrimeleNFructe(fructe, nrFructe, nrCopieFructe);
	modificaDenumire(&copieFructe[5], "Mar copiat");
	afisareVectorFructe(copieFructe, nrCopieFructe);

	struct Fruct* fructeVechi = NULL;
	int nrFructeVechi = 0;
	copiazaFructeVechi(fructe, nrFructe, 10, &fructeVechi, &nrFructeVechi);
	afisareVectorFructe(fructeVechi, nrFructeVechi);

	dezalocareVectorFructe(&copieFructe, &nrCopieFructe);
	afisareVectorFructe(copieFructe, nrCopieFructe);
	return 0;
}