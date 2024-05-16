#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

typedef int pipe_t[2];      /* definizione del TIPO pipe_t come array di 2 interi */

int main(int argc, char** argv)
{
    /* ------------- Variabili locali ------------- */

    int pid;                /* process identifier per le fork() */
    int N;                  /* numero di file passati sulla riga di comando, che corrispondera' al numero di figli */
    pipe_t* piped;          /* array dinamico di pipe descriptors per comunicazioni figli-padre  */
    int i, j;               /* indici per i cicli */
    int lunghezza;          /* variabile che viene comunicata da ogni figlio al padre */
    int status;			    /* variabile di stato per la wait */
    int ritorno;            /* variabile usata dal padre per recuperare valore di ritorno di ogni figlio */

    /* -------------------------------------------- */

    /* Controllo sul numero di parametri */
    if (argc < 3)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 3\n", argc);
        exit(1);
    }

    /* Numero di parametri passati da linea di comando */
    N = argc - 1;

    /* Allocazione dell'array di pipe */
    piped = (pipe_t*) malloc(N * sizeof(pipe_t));
    if (piped == NULL)
    {
        printf("Errore nell'allocazione della memoria per il vettore di pipe.\n");
        exit(2);
    }

    /* Creazione delle N pipe */
    for (i = 0; i < N; i++)
    {
        if (pipe(piped[i]) < 0)
        {
            printf("Errore nel piping.\n");
            exit(3);
        }
    }

    /* Ciclo di generazione dei figli */
    for (i = 0; i < N; i++)
    {
        if ((pid = fork()) < 0)
        {
            printf("Errore nella fork.\n");
            exit(4);
        }

        if (pid == 0)
        {
            /* Codice del figlio */
            printf("DEBUG-Sono il processo figlio di indice %d e pid %d sto per creare il nipote che contera' le linee del file %s\n", i, getpid(), argv[i+1]);

            /* Chiusura di tutte le pipe non utilizzate nella comunicazione con il padre */
            for (j = 0; j < N; j++)
            {
                /* Chiudo il lato della pipe non usato dai figli */
                close(piped[j][0]);
                /* Per ogni figlio tengo aperto il lato per la scrittura solo se j == i perche' gli altri canali di comunicazione sono riservati agli altri figli */
                if (j != i)
                {
                    close(piped[j][1]);
                }
            }

            /* Apro una pipe per la comunicazione con il nipote */
            pipe_t pipednipote;
            if (pipe(pipednipote) < 0)
            {
                printf("Errore nel piping.\n");
                exit(6);
            }

            /* Creazione del nipote */
            if ((pid = fork()) < 0)
            {
                printf("Errore nella fork.\n");
                exit(5);
            }

            if (pid == 0)
            {
                /* Codice del nipote */
                /* Ridirigo lo standard output su pipednipote[1] */
                close(1);
                dup(pipednipote[1]);
                
                /* Chiudo il lato di pipe il nipote non utilizza */
                close(pipednipote[0]);
                /* Chiudo il lato di pipe gia' duplicato */
                close(pipednipote[1]);

                /* Redirigo stdin */
                int fd;
                close(0);
                if ((fd = open(argv[i + 1], O_RDONLY)) < 0)
                {
                    printf("Errore nell'apertura del file");
                    exit(9);
                }

                /* Eseguo il wc -l */
                execlp("wc", "wc", "-l", (char*)0);

                /* Se arrivo qui la exec ha fallito */
                printf("Errore nella exec.\n");
                exit(7);
            }

            /* Codice del figlio */
            /* Chiudo il lato della pipe non utilizzato */
            close(pipednipote[1]);

            /* Aspetto il nipote per assicurarmi di avere qualcosa da leggere dalla pipednipote */
            if ((pid = wait(&status)) < 0)
            {
                printf("Errore nella wait del figlio.\n");
                exit(8);
            }
            if ((status & 0xFF) != 0)
            {
                printf("Nipote con pid %d terminato in modo anomalo\n", pid);
            }	
    		else
		    { 	
                ritorno = (int)((status >> 8) &	0xFF); 
		      	if (ritorno != 0)
                {
                    printf("Il nipote con pid = %d ha ritornato %d e quindi vuole dire che il nipote non e' riuscito ad eseguire il wc -l oppure e' incorso in un errore\n", pid, ritorno);
                }
		      	else
                {
                    printf("Il nipote con pid = %d ha ritornato %d\n", pid, ritorno);
                }  	
		    }
            
            /* Leggo dalla pipednipote il valore di ritorno di wc -l */
            read(pipednipote[0], &lunghezza, sizeof(lunghezza));
            /* Scrivo il valore su piped[i][1] per comunicarlo al padre */
            write(piped[i][1], &lunghezza, sizeof(lunghezza));
        }
    }

    /* Codice del padre */
    /* Chiudo i lati di scrittura della pipe non usati dal padre */
    for (j = 0; j < N; j++)
    {
        close(piped[j][1]);
    }
    /* Aspetto tutti i figli */
    for (i = 0; i < N; i++)
    {
        if ((pid = wait(&status)) < 0)
        {
            printf("Errore nella wait del padre.\n");
            exit(8);
        }
        if ((status & 0xFF) != 0)
        {
            printf("Figlio con pid %d terminato in modo anomalo\n", pid);
        }	
    	else
	    { 	
            ritorno = (int)((status >> 8) &	0xFF); 
	      	if (ritorno != 0)
            {
                printf("Il figlio con pid = %d ha ritornato %d. Cio' significa che il nipote non e' riuscito ad eseguire il wc -l oppure il figlio o il nipote sono incorsi in errori\n", pid, ritorno);
            }
	      	else
            {
                printf("Il figlio con pid = %d ha ritornato %d\n", pid, ritorno);
                /* Leggo dalla piped[0] il valore scritto dal figlio */
                read(piped[i][0], &lunghezza, sizeof(lunghezza));
            }  	
	    }
    }
    exit(0);
}