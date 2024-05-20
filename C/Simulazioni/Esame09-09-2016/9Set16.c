#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

typedef int pipe_t[2];         /* definizione del TIPO pipe_t come array di 2 interi */

int main(int argc, char** argv)
{
    /* ------------- Variabili locali ------------- */
    
    int pid;				/* process identifier per le fork() */
    int N;					/* numero di file passati sulla riga di comando */
    int i,j;				/* indici per i cicli */
    pipe_t *piped;			/* array dinamico di pipe descriptors */
    pipe_t p;               /* pipe per la comunicazione tra 26 esimo figlio e padre */
    int status;				/* variabile di stato per la wait */
    int ritorno;			/* variabile usata dal padre per recuperare valore di ritorno di ogni figlio */
    
    /* -------------------------------------------- */

    if (argc != 2)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 2\n", argc);
        exit(1);
    }
    
    /* Numero di parametri passati da linea di comando */
    N = argc - 1;

    /* Allocazione dell'array di pipe */
    piped = (pipe_t*)malloc(26 * sizeof(pipe_t));
    /* Controllo che l'allocazione di memoria sia andaata a buon fine */
    if (piped == NULL)
    {
        /* In tal caso si e' verificato un errore nell'allocazione: stampo un messaggio d'errore ed esco specificando un valore intero d'errore */
        printf("Errore nell'allocazione della memoria per il vettore di pipe.\n");
        exit(2);
    }
    
    /* Creazione delle 26 pipe per la comunicazione tra figli */
    for (i = 0; i < 26; i++)
    {
        /* Controllo che ogni creazione vada a buon fine */
        if (pipe(piped[i]) < 0)
        {
            /* Non e' stato possibile creare la pipe con successo: stampo un messaggio d'errore ed esco specificando un valore intero d'errore */
            printf("Errore nel piping.\n");
            exit(3);
        }
    }

    /* Creo una pipe per consentire la comunicazione tra padre e ultimo figlio verificando se l'operazione va a buon fine */
    if ((pipe(p)) < 0)
    {
        /* La creazione della pipe ha fallito, stampo un messaggio d'errore ed esco specificando un valore intero d'errore */
        printf("Errore nel piping.\n");
        exit(3);
    }
    
    /* Generazione dei 26 figli */
    for (i = 0; i < 26; i++)
    {
        /* Genero un processo figlio */
        /* Controllo che la fork() abbia successo */
        if ((pid = fork()) < 0)
        {
            /* La fork() ha fallito, dunque stampo un messaggio d'errore e ritorno un valore intero d'errore */
            printf("Errore nella fork.\n");
            exit(4);
        }
        
        /* Se pid == 0, allora la fork() ha avuto successo e possiamo eseguire il codice del figlio */
        if (pid == 0)
        {
            /* Codice del figlio */
            printf("DEBUG-Esecuzione del processo figlio %d\n", getpid());
            
            /* Chiudo i lati di pipe non utilizzati dal figlio */
            for (j = 0; j < 26; j++)
            {
                if (j == 25) 
                {
                    
                }
                if (j == 0 || (i - 1) != (j - 1))
                {
                    close(piped[j][0]);
                }
                if (i != j)
                {
                    close(piped[j][1]);
                }
            }
            
            
        }
    }
    
    /* Codice del padre */
    /* Chiudo il lato di pipe non usato dal padre */
    for (i = 0; i < 26; i++)
    {
        close(piped[i][1]);
    }
    
    
    
    exit(0);
}