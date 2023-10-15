#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include "serv_cli_fifo.h"
#include "Handlers_Serv.h" 


int main(int argc, char *argv[])   
{
  printf("\033[35m" " \n ************************  SERVEUR ************************** \n \n " ) ;  
  printf("\033[37m" " " ) ;

	/* Déclaration */
	int j;
	int y=1;
	Question qst;
	Reponse rps;
	int mk;
	int mk2;
  
	/* Création des tubes nommés */
	mk = mkfifo(FIFO1, 0777);
	mk2 = mkfifo(FIFO2, 0777);

	/*initialisation du générateur de nombres aléatoires*/
	srand(getpid());
	
	
	
	/* initialisation de Handler reveil  */
	void hand_reveil(int sig){
		printf(" le client(%d) : la reponse a été reçu avec " "\033[1m""succès \n" ,qst.pid);
	}
	/* traitement géneré lorsque le serveur est arreté suite a une interruption */ 
	void fin_serveur(int sig){
		/* liberation d'allocation */
		free(rps.tab);
		unlink(FIFO1);
		unlink(FIFO2);
		y=0;
		printf("\033[31m"" \n *********************  Server Shutdown   ********************" "\n");
		exit(1);
	}
	
	
	

	while(y){
	
		
		
		
		/* Installation de Handler */
		for(j=1;j<NSIG;j++){ 
		/* initialisation du signal */
		signal(j,fin_serveur);
		} 
		signal(SIGUSR1,hand_reveil);
		
		
		/* Ouverture du tube nommé */
		int fdread = open(FIFO1, O_RDONLY); 
		/* lecture du question du client */
		read(fdread, &qst, sizeof(Question)); 
		/* Fermeture du tube nommé */
		close(fdread);  
		/* Affichage du question */
		printf("La Question du Client %d est : %d.\n \n ",qst.pid,qst.n);
		
		/* construction de la réponse */ 
		rps.pid = getpid();
		rps.tab = malloc( sizeof(int) * qst.n );
		for (j = 0; j < qst.n; j++)
		{
			rps.tab[j] = NMIN + rand() % (NMAX+1 - NMIN);
		};
		
		/* envoi du signal SIGUSR1 au client concerné */
		sleep(1);
		kill(qst.pid,SIGUSR1);
		
		/* envoi de la réponse */
		/* Ouverture du tube nommé */
		int fdwrite2 = open(FIFO2, O_WRONLY); 
		/* envoi de la réponse */
		write(fdwrite2, &rps.pid, sizeof(int));
		write(fdwrite2, rps.tab, (sizeof(int) * qst.n) ); 
		/* fermeture du tube */ 
		close(fdwrite2);
		
		
	}
	
	return 0;
}

