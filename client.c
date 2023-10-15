#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include "serv_cli_fifo.h"
#include "Handlers_Cli.h"

int main(int argc, char *argv[])
{     
          printf("\033[35m" " \n ************************  CLIENT ************************** \n " ) ; 
          printf("\033[37m" " " ) ;  
          
          
	/* Déclaration */
	Question qst;
	Reponse rps;
	int i;
	
	/*initialisation du générateur de nombres aléatoires*/
	srand(getpid());
	
	
	/* handler reveil  */ 
	void hand_reveil(int sig){
	
 
	/* allocation de la memoire */
	rps.tab = malloc( sizeof(int) * qst.n );
	
	/* Ouverture des tubes nommés */
	int fdread2 = open(FIFO2, O_RDONLY); 
	/* Récuperation de le reponse */
	read(fdread2, &rps.pid, sizeof(int));
	read(fdread2, rps.tab, sizeof(int) * qst.n); 
	/* Fermeture du tube */ 
	close(fdread2);
	/* se mettre en attente jusqu'a l'arrivée de la réponse */ 
	sleep(1); 
	/* Avertir le serveur que la reponse a été recu avec un signal */
	kill(rps.pid,SIGUSR1);
	
	/* Affichage de la reponse */
	printf(" La Reponse reçu de la part du Serveur:\n");
	for (i = 0; i < qst.n; i++)
	{
		printf("- %d .\n",rps.tab[i]);
	};
	
	/* liberation d'allocation */
	free(rps.tab);
	}
	
	
	
	/* Installation des Handlers */
	signal(SIGUSR1,hand_reveil);
	
	/* Construction et envoi du question */
	qst.n = NMIN + rand() % (NMAX+1 - NMIN);
	qst.pid = getpid();
	
	/* Ouverture du tubeu nommé */
	int fdwrite = open(FIFO1, O_WRONLY); 
	/* Ecriture du question dans le tube */
	write(fdwrite, &qst, sizeof(Question)); 
	/* Fermeture du tube */ 
	close(fdwrite);
	
	/* Attente de la réponse */
	pause();
	sleep(1);
	
	
	
	
	

	return 0;

	
}

