#include<stdio.h>
#include<malloc.h>
#include<string.h>

//Chiritescu Valentin -> C si V -> Articol Covrig

struct Covrig {
	int id;
	char* denumire;
	float pret;
	int nrIngrediente;
	char** ingrediente;
	int* gramajeIngrediente;
};

int main() {
	struct Covrig c1;
	c1.id = 1;
	c1.denumire = "Covrig cu sare";
	c1.pret = 1.99f;
	c1.nrIngrediente = 5;
	c1.ingrediente = (char*)malloc(c1.nrIngrediente * sizeof(char*));
	c1.ingrediente[0] = "faina";	//50
	c1.ingrediente[1] = "apa";		//30
	c1.ingrediente[2] = "ulei";		//10
	c1.ingrediente[3] = "drojdie";	//10
	c1.ingrediente[4] = "sare";		//25
	int gramajeIngrediente[5] = {50, 30, 10, 10, 25};
	c1.gramajeIngrediente = gramajeIngrediente;

	for (int i = 0; i < c1.nrIngrediente; i++) {
		printf("%s\t\t%d\n", c1.ingrediente[i], c1.gramajeIngrediente[i]);
	}

	return 0;
}