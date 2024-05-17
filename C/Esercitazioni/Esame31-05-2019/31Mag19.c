#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];         /* definizione del TIPO pipe_t come array di 2 interi */

int main(int argc, char** argv)
{
    /* ------------- Variabili locali ------------- */
    
    int pid;				/* process identifier per le fork() */
    int N;					/* numero di file passati sulla riga di comando */
    int i,j;				/* indici per i cicli */
    pipe_t *piped;			/* array dinamico di pipe descriptors */
    pipe_t pipednipote;     /* pipe per la comunicazione tra figlio e nipote */
    int status;				/* variabile di stato per la wait */
    int ritorno;			/* variabile usata dal padre per recuperare valore di ritorno di ogni figlio */
    
    /* -------------------------------------------- */

    if (argc < 3)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 3\n", argc);
        exit(1);
    }
    
    /* Numero di parametri passati da linea di comando */
    N = argc - 1;
    
    /* Allocazione dell'array di pipe */
    piped = (pipe_t*)malloc(N * sizeof(pipe_t));
    /* Controllo che l'allocazione di memoria sia andaata a buon fine */
    if (piped == NULL)
    {
        /* In tal caso si e' verificato un errore nell'allocazione: stampo un messaggio d'errore ed esco specificando un valore intero d'errore */
        printf("Errore nell'allocazione della memoria per il vettore di pipe.\n");
        exit(2);
    }
    
    /* Creazione delle N pipe */
    for (i = 0; i < N; i++)
    {
        /* Controllo che ogni creazione vada a buon fine */
        if (pipe(piped[i]) < 0)
        {
            /* Non e' stato possibile creare la pipe con successo: stampo un messaggio d'errore ed esco specificando un valore intero d'errore */
            printf("Errore nel piping.\n");
            exit(3);
        }
    }
    
    /* Generazione degli N figli */
    for (i = 0; i < N; i++)
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
            for (j = 0; j < N; j++)
            {
                close(piped[i][0]);
                if (i != j) 
                {
                    close(piped[i][1]);
                }
            }
            
            /* Creo una pipe per consentire la comunicazione figlio-nipote verificando se l'operazione va a buon fine */
            if ((pipe(pipednipote)) < 0)
            {
                /* La creazione della pipe ha fallito, stampo un messaggio d'errore ed esco specificando un valore intero d'errore */
                printf("Errore nel piping.\n");
                exit(3);
            }
            
            /* Genero un processo nipote */
            /* Controllo che la fork() abbia successo */
            if ((pid = fork()) < 0)
            {
                /* La fork() ha fallito, dunque stampo un messaggio d'errore e ritorno un valore intero d'errore */
                printf("Errore nella fork.\n");
                exit(4);
            }
            
            /* Se pid == 0, allora la fork() ha avuto successo e possiamo eseguire il codice del nipote */
            if (pid == 0)
            {
                /* Codice del nipote */
                printf("DEBUG-Esecuzione del processo nipote %d\n", getpid());
                
                /* Chiudo il lato di pipe non usato dal nipote */
                close(pipednipote[0]);
                /* Chiudo il lato di pipe ereditato dal figlio che lo utilizza per comunicare al padre */
                close(piped[i][1]);

                /* Eseguo il sort del file corrispondente */
                execlp("sort", "sort", "-f", argv[i +1], (char*)0);

                /* Se torno qui ho avuto un errore con la exec */
                printf("Errore nella exec.\n");
                exit(-1);
            }
            
            /* Codice del figlio */
            /* Chiudo il lato di pipe non usato dal figlio */
            close(pipednipote[1]);
        }
    }
    
    /* Codice del padre */
    /* Chiudo i lati di pipe non utilizzati dal padre */
    for (i = 0; i < N; i++)
    {
        close(piped[i][1]);
    }
    
    
    exit(0);
}

/*
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWWWMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWWWWXOdllokKXNWWWWWNXXNNXNWMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWNK00Okkxoc;'....';:ldddddolooloxxOXNWMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWXOxddoc:;;:;;,..''...',;::clllc::clcclx0WM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWKkoccolc;,,;:;:cc;,,'':llooodxxxdoolooc::lkN
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNklc:::c::::;,::;coocclodxxooooddddooolc;,;:oX
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWKo;;:::;;;;;;,;:::::clloddol:;;:clcc:;;;;;;cdXW
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWWWWWWWWWWWWWMMMMMMMMMMMWKxc:;::::;;,,;'';;,....';:;,''..'clclc;:::dxx0NMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWWWWWWMMMMMMMMWWWNNXXXXXXKKK00OOO0KXXNNNNNXXX0koc::::ccc:;,'',;::;,........;c,.oNWNNX0KKXWWWMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWNK0OkkOOkO0KXXXXXXXKK0KKKKKXXK00OOkkxkkOOO0Okxddxxxdl:;::;::c:,'',;cloc:;'...',cdo',OMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWX0OxdooolooddxxxkxxxkkxxdddkOO000KK0Okxkxkkkxxkkkxdddoolc::;;;;:c;'',,,cdxdl:;;''',:cc',ONWMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWX0xollllloodxxxxxddddddddoodddkOOOOOO0KK0kkkkOkxddddooolccccc::;;:;,,,'...;lodolc::;,,;;;,,cox0NWMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWNNXK0kdollooooddoodxxddddddxddxkkkxkO000kxxxkkOxdxxkkkkkxdollcccccc::;,,'',,'',..,:clolc:;;clc;,,,,:okOXWMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWNXK0OkxxxdoooddxxxkOkkxxddollooodxkO00OkO00OOkddddxxddodxxkO00Oxoc::::cc:,....',,;;;....';:llc::lol:::;,cxk0NWMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMWNXKKOkdooodddddoodddodddxkkOOOkxdlc:ccodxk00OO0OkkxxdoddddoolloddxO0K0Odccc:cc:,''',,;:;;'...'cdollc:;;::clc:,,,;ok0XNMMMMMMM
MMMMMMMMMMMMMMMMMMWWNXK0Okddooolllodddxxdooollllodxxxkkxxdol:;;cldxOOkkxdddolcllllllllcclodxkkkkkdollc::;;;,''''....,ckNMMWNK0Okxdolc::;;::ccldKWMMMMM
MMMMMMMMMMMWWNXXK00Okxddoooollccllodddddoooolcccloodooollllc::::codxxddlcolccccccccccccclloolllllc::;;;;,,,'.....;lx0NWMMMMMMMMMMMWX0kdl::clldOXMMMMMM
MMMMMMMWXKK0Okxxxxxddoolllccc::cllloodxdollolccccllllllc:::;;;;;;:llooc;;:;;::::::;;:ccccc:::;:::;,,;;,'....';ldOXWMMMMMMMMMMMMMMMMMMMWNK000XNMMMMMMMM
MMMMWX0kddddddddddddddolcc:::::cccclodxddoloolcccccccc::::;;,'',,,;;:c:;:::::c::;;,,,,;:c:,,,,;;;,'',,,;coxk0XWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMWKkdooollllooooddddollcc::::;::::clodddolloolccccccccccc:;,'''',,;;::ccccccc:;,'''.',;;,;;;,''...',;o0NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MNOoolllccccccccllllllc::;;;;,,;;;;;;:cloooooddolcccccccccc::;;;;:::::cccc::;;,,'''...',;;;,'.....,;:dXMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
WOllc::::;;;;::::::::::;;,,,'''',,,,''',::llccllllllllllllccccclllc:;,,,;;;;;;,,'''..':lc,.....'',;ckNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
Xd:::;;;::ccccccccllcccc:::;;,',,,,,'....,;;;,,;;:cllllclooooooolcc:;,',;:;;;;,,'''''',:c:'...'::,':kNWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
Xd:;;cxOKXXXXXXXNNNNNNNNXXXKK000KKK0Okd;....''''''',:::::codxxxolc::::,',,,''''''''''';:llc:;;:dOkc',cok0XXKOkOXWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
WO:;l0WMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWNOd:,'............,codollcc:;;;,.........';:oxOKNNKkdc:oOXWN0xdolloolc;l0WMMMM $ echo DEBUG-AAAAAARRRRRR MMMMMM
MNk:lKWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNK00Okxoc,.....cddol::;;;,,,'...',;:ldkKNWMMMMMMMWXxodOWMMMMMWWNNNNkoOWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMW0odKWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWXOd:':ddlc:;,,;;,,:dkk0KXXWWMMMMMMMMMMMMMMWX00NMMMMMMMMMMMMNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMN0k0KNWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNOollc:;,,,,,'':OWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMWXK00KXWWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWXko:,;:::;,''''.':OWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMWNKKKKKKXNWWMMMMMMMMMMMMMMMMMMMMMMMMMWXOo;'...',;,'...'..c0WMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMWNNXXNNNWMMMMMMMMMMMMMMMMMNKOkxoc;''.....','.......'dWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXxc,............',,'.......cKMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXo'.............',,'...';cokXWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMW0c.......;loddo:,;,'...:kXWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWx,......l0NWMMWKoc;,'..lXMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl......lXMMMMMXd:c:,.'oXMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMKc......,kWMMMMMWkc;,''cKMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMKc.......cKMMMMMMMWkc;,,c0WMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMO,.......lXMMMMMMMMNx:,,:dXMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNd'.... .cKMMMMMMMMMKo:,,cxNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNo......,x0XWMMMMMMNd:,';coONWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMWWWWWWWWWWWWMMMMMMMXo.......';lONMMMMNd;,'';:cokOKKKKXWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMWNNNXXXKKKKKKKKKKKKKOc,'',,,,;;ckKKKKk:','..,;;;::c:;codkKWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMWWWWWNNNNXXXXKK0OOOOOkkkkkkkkxxc'.''.....'','...',;lkKKKXXXXXXXXXXXXXXNNNNNNNNNNNWWWWWWWWWWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWWWWWWNXKK0OOkdc;'..':lllcc:;;:ldkOO0000000KKKKXXXXXXXNNNNNNNNNNNNNNWWWWWWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWWWWNNNXKkkOXNNNNNNNNNWWWWWWWWWWWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
*/