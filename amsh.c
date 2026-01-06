#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

typedef struct {
	char sursa[70];
	char destinatie[70];
	int limita_timp;
	long ultima_accesare;
	int e_montat; // 0 - nu e montat, 1 - e montat
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
		lista_montari[nr_montari].e_montat = 0;
		nr_montari++;
	}
	fclose(fisier);
	printf("S-au incarcat %d configuratii din fisier.\n", nr_montari);
}

void initializare_timp_lavinia(){
	time_t acum = time(NULL);
	for (int i = 0; i < nr_montari; i++){
		lista_montari[i].ultima_accesare = acum ;
	}
	printf(" Toate mountpointurile au fost initializate cu timpul curent.\n");
}
void afisare_status_timer_lavinia(){
	time_t acum = time(NULL);
	printf("STATUS TIMP:\n");
	for ( int i = 0; i < nr_montari; i++){
		double secunde_trecute = difftime(acum, lista_montari[i].ultima_accesare);
		printf("Punct: %s | Limita: %d s | Montat de: %0.f s\n",
			 lista_montari[i].destinatie,
			 lista_montari[i].limita_timp,
			 secunde_trecute);
		if (secunde_trecute > lista_montari[i].limita_timp ){
			printf("EXPIRAT - necesita demontare\n");
		}else {
			printf("OK\n");}
	}
}

void actualizeaza_timp_acces_lavinia( char *cale_accesata){
	if (cale_accesata == NULL) return;
	for (int i=0; i< nr_montari; i++){
		if(strcmp(cale_accesata, lista_montari[i].destinatie) == 0){
			lista_montari[i].ultima_accesare = time(NULL);
			printf("Cronometru resetat pentru: %s\n", cale_accesata);
			return;
		}
	}
}

int demontare(char *destinatie){
	char cmd[70];
	strcpy(comanda, "demontare");
	strcat(comanda, destinatie);
	int rezultat = system(comanda);
	if (rezultat == 0){
		printf("Demontare efectuata pentru %s\n", destinatie);
	}else{
		printf("Demontare nereusita pentru %s\n", destinatie);
	}
	return rezultat;
}

int main(){
	char line[1024];
	char *comanda;
	char *argument;
	int lungime;

	configuratie("test_configurare");
	initializare_timp_lavinia();
	while(1) {
		printf("\n");
		printf("amsh> ");
                fflush(stdout);

		if (fgets(line, sizeof(line), stdin) == NULL) {
			break;
			}
	
		lungime = strlen(line);
		if (lungime > 0 && line[lungime-1] == '\n') {
 			line[lungime - 1] = '\0';
			}
		if (strlen(line) == 0) continue;

		if (strcmp(line, "exit") == 0) {
			break;
			}

		comanda = strtok(line, " ");
		argument = strtok(NULL, " ");


		if (comanda != NULL && strcmp(comanda, "cd") == 0) {
			if (argument!=NULL){
				for (int i = 0, i< nr_montari;i++){
					if(strcmp(argument, lista_montari[i].destinatie) == 0){
						if (lista_montari[i].e_montat ==1){
							actializeaza_timp_access_lavinia(argument);
						}
						break;
					}
				}
				if (chdir(argument) == 0){
					printf("Director schimbat cu succes.\n");
					afisare_status_timer_lavinia();
				}else{
					perror("amsh: cd failed");
				}
			}else {
				printf("Eroare:  cd are nevoie de o cale.\n");
			}
		}

		else if (strcmp(comanda, "demontare") == 0) {
			if  (argument == NULL){
			printf(Eroare: Comanda demontare nu a primit o cale.\n");
			}else{
				int gasit = -1;
				for (int i =0;i<nr_montari, i++){
					if (strcmp(lista_montari[i].destinatie, argument) == 0 &&lista_montari[i].e_montat == 1){
					gasit =i;
					break;
					}
				}
			if (gasit == -1){
			printf("Mountpoint-ul %s nu exista sau nu este activ.\n", argument);
			}else{
			if(demontare(argument)==0){
				lista_montari[gasit].e_montat = 0;
				lista_montari[gasit].ultima_accesare = 0;
				print("Mountpoint_ul 5s a fost demontat manual.\n", argument);
			}
			}
			}
		}

		else if (strcmp(comanda, "show_active") == 0) {
		            afisare_status_timer_lavinia();}

		else if (strcmp(comanda, "lista") == 0){
				printf("Lista de mountpoint-uri active:\n");
                                for (int i = 0; i<nr_montari; i++){
                                        printf("%d. %s -> %s (%d secunde)\n", i+1, lista_montari[i].sursa, lista_montari[i].destinatie, lista_montari[i].limita_timp);
                                }

			}
			else {
				pid_t pid = fork();
				if (pid==0){
						char *args[] = {comanda, argument, NULL};
					   	if (execvp(comanda, args) == -1) {
                					perror("amsh");
							}
						exit(EXIT_FAILURE);
					    }
				else if (pid>0){
						wait(NULL);
						}
				else{
					perror("fork failed");
				    }
				}
		}
	return 0;
}

