#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(){
	char line[1024];
	char *comanda;
	char *argument;
	int lungime;
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
				printf("Ai cerut comanda interna cd\n");
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
