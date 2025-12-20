#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct {
	char sursa[70];
	char destinatie[70];
	int limita_timp;
	long ultima_accesare;
}Montare;

Montare lista_montari[70];
int nr_montari = 0;

void configuratie(char *fisier_citire){
	FILE *fisier = fopen(fisier_citire, "r");
	if(fisier ==NULL) {
	printf("Nu exista date in fisier");
	}
	nr_montari = 0;
	while(fscanf(fisier, "%s %s %d",lista_montari[nr_montari].sursa, lista_montari[nr_montari].destinatie, &lista_montari[nr_montari].limita_timp) == 3)
	{
		lista_montari[nr_montari].ultima_accesare = 0;
		nr_montari++;
	}
	fclose(fisier);
	printf("S-au incarcat %d configuratii din fisier.\n", nr_montari);
}

int main(){
	char line[1024];
	char *comanda;
	char *argument;
	int lungime;

	configuratie("test_configurare");
	while(1) {
		printf("amsh> ");
		fflush(stdout);

		if (fgets(line, sizeof(line), stdin) == NULL) {
			break;
			}

		lungime = strlen(line);
		if (lungime > 0 && line[lungime-1] == '\n') {
 			line[lungime - 1] = '\0';
			}

		if (strcmp(line, "exit") == 0) {
			break;
			}

		comanda = strtok(line, " ");
		argument = strtok(NULL, " ");

		if (comanda != NULL) {

			if (strcmp(comanda, "cd") == 0) {
				printf("Lista de mountpoint-uri active:\n");
				for (int i = 0; i<nr_montari; i++){
					printf("%d. %s -> %s (%d secunde)\n", i+1, lista_montari[i].sursa, lista_montari[i].destinatie, lista_montari[i].limita_timp);
				}				
					
			}
			else {
				printf("Ai cerut comanda externa: %s\n",comanda);
				}
			if (argument) {
					printf("Cu argumentul: %s\n", argument);
					}
			}
		}
return 0;
}

