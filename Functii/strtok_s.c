#include<stdio.h>
#include<malloc.h>
#include<string.h>

int main() {
	char stringInStack[] = "Acest string este in stack";
	char* stringInHeap = (char*)malloc(strlen("Acest string este in heap") + 1);
	strcpy_s(stringInHeap, strlen("Acest string este in heap") + 1, "Acest string este in heap");

	printf("char stringInStack[] \"Acest string este in stack\";\n");
	printf("print %%p  stringInStack: %p\n", stringInStack);
	printf("print %%s  stringInStack: %s\n", stringInStack);
	printf("print %%c *stringInStack: %c\n\n", *stringInStack);

	char* nextPointer = NULL;
	char* token = strtok_s(stringInStack, " ", &nextPointer);
	printf("Token: %s \t nextPointer: %s\n", token, nextPointer);
	while (*nextPointer != NULL) {
		token = strtok_s(NULL, " ", &nextPointer);
		printf("Token: %s \t nextPointer: %s\n", token, nextPointer);
	}
	printf("print %%s  stringInStack modificat de strtok_s: %s\n\n", stringInStack);

	printf("===========================\n\n");

	printf("char* stringInHeap \"Acest string este in heap\"\n");
	printf("print %%p  stringInHeap: %p\n", stringInHeap);
	printf("print %%s  stringInHeap: %s\n", stringInHeap);
	printf("print %%c *stringInHeap: %c\n\n", *stringInHeap);

	nextPointer = NULL;
	token = strtok_s(stringInHeap, " ", &nextPointer);
	printf("Token: %s \t nextPointer: %s\n", token, nextPointer);
	while (*nextPointer != NULL) {
		token = strtok_s(NULL, " ", &nextPointer);
		printf("Token: %s \t nextPointer: %s\n", token, nextPointer);
	}
	printf("print %%s  stringInHeap modificat de strtok_s: %s\n\n", stringInHeap);

	for (int i = 0; i < strlen("Acest string este in heap") + 1; i++) {
		if (stringInHeap[i] == NULL) {
			printf("stringInHeap[%d] NULL\n", i);
		}
		else {
			printf("stringInHeap[%d] %c\n", i, stringInHeap[i]);
		}
	}


	return 0;
}