#!/bin/sh

#########################################################################
#                           DEPRECATED                                  #
#########################################################################

### LEGENDA RETURN VALUES ###

# 0: Tutto e' andato a buon fine
# 1: Numero parametri errato
# 2: Il parametro non e' un path assoluto
# 3: Il parametro non e' una directory
# 4: Il parametro non e' una directory traversabile
# 5: Il parametro non e' un numero intero
# 6: Il numero non e' positivo

# -------------------------------------------------------------------------------------------------------------- #

### Aggiornamento del PATH ###

# Aggiungo alla variabile d'ambiente PATH il path corrente
PATH=`pwd`:$PATH
# Aggiungo le modifiche effettuate alla variabile d'ambiente esportandola
export PATH

# -------------------------------------------------------------------------------------------------------------- #

### Controllo PARAMETRI STRETTO ###

# Controllo che il numero di parametri rientri nella soglia desiderata
if test $# -ne 3; then
    # In tal caso il numero dei parametri non riesntra nell'intervallo desiderato, quindi interrompo l'esecuzione
    echo "Numero parametri errato. Processo interrotto."
    # Esco specificando un valore intero di errore
    exit 1;
fi

# -------------------------------------------------------------------------------------------------------------- #

### Controllo PARAMETRI LASCO ###

# Controllo che il numero di parametri rientri nella soglia desiderata
if test $# -lt 3; then
    # In tal caso il numero dei parametri non riesntra nell'intervallo desiderato, quindi interrompo l'esecuzione
    echo "Numero parametri errato. Processo interrotto."
    # Esco specificando un valore intero di errore
    exit 1;
fi

# -------------------------------------------------------------------------------------------------------------- #

### Verifica che un NUMERO sia INTERO POSITIVO ###

# Controllo se il parametro e' un numero intero
case $i in
*[!0-9]*)
    # In tal caso il parametro non e' un numero intero, quindi interrompo l'esecuzione
    echo "Il parametro i non e' un numero intero. Processo interrotto."
    # Esco specificando un valore intero di errore
    exit 5;;
*)
    echo "Il parametro $i e' un numero intero."
    # Controllo se il numero e' positivo
    if test "$i" -le 0; then
        echo "Il numero $i non e' positivo."
        # Esco specificando un valore intero di errore
        exit 6
    else
        echo "Il numero $i e' positivo."
    fi;;
esac

# -------------------------------------------------------------------------------------------------------------- #

### Verifica che un PARAMETRO sia una STRINGA ###

# Controllo che il parametro $1 sia una stringa
case $1 in
    *[0-9]*)
        echo "Il parametro $1 non e' una stringa. Processo interrotto."
        # Esco specificando un valore intero di errore
        exit 9;;
    *)
        echo "Il parametro  e' una stringa.";;
esac

# -------------------------------------------------------------------------------------------------------------- #

### Verifica che un PARAMETRO sia un CARATTERE ###

# Verifico che il parametro $1 sia un carattere
case $1 in
    [a-z,A-Z])
        echo "Il primo parametro $1 e' un carattere.";;
    *)
        echo "Il primo parametro $1 non e' un carattere. Processo interrotto."
        # Esco specificando un valore intero di errore
        exit 2;;
esac

# -------------------------------------------------------------------------------------------------------------- #

### Verifico che il PATH sia ASSOLUTO ###
### Verifico che se la DIRECTORY e' ESISTENTE ###
### Verifico che se la DIRECTORY e' TRAVERSABILE ###

# Uso un case per verificare se il parametro e' un path assoluto
case $i in
/*)
    echo "Il parametro $i e' un path assoluto."
    # Ora verifico l'esistenza della directory
    if test -d "$i"; then
        echo "Il parametro $i e' una directory."
        # Infine verifico che la directory sia traversabile
        if test -x "$i"; then
            echo "Il parametro $i e' una directory traversabile."
        else
            echo "Il parametro $i non e' una directory traversabile. Processo interrotto."
            # Esco specificando un valore intero di errore
            exit 4
        fi
    else
        echo "Il parametro $i non e' una directory. Processo interrotto."
        # Esco specificando un valore intero di errore
        exit 3
    fi;;
*)
    echo "Il parametro $i non e' un path assoluto. Processo interrotto."
    # Esco specificando un valore intero di errore
    exit 2;;
esac

# -------------------------------------------------------------------------------------------------------------- #

### Verifico che il PATH sia ASSOLUTO ###
### Verifica combinata: DIRECTORY ESISTENTE && DIRECTORY TRAVERSABILE ###

# Uso un case per verificare se il parametro e' un path assoluto
case $i in
/*)
    echo "Il parametro $i e' un path assoluto."
    # Ora verifico l'esistenza della directory e che essa sia traversabile
    if test -d "$i" -a -x "$i"; then
        echo "Il parametro $i e' una directory traversabile."
    else
        echo "Il parametro $i non e' una directory traversabile. Processo interrotto."
        # Esco specificando un valore intero di errore
        exit 3
    fi;;
*)
    echo "Il parametro $i non e' un path assoluto. Processo interrotto."
    # Esco specificando un valore intero di errore
    exit 2;;
esac

# -------------------------------------------------------------------------------------------------------------- #

### Ciclo con CONTATORE ###

# Dichiaro una variabile da usare come contatore
n=1
# Scorro la lista in un ciclo che utilizza il contatore n
for G; do
    # Incremento la variabile contatore di 1 a ogni iterazione
    n=`expr "$n" + 1`
done
# Reimposto il contatore al punto di partenza, ovvero a 1
n=1

# -------------------------------------------------------------------------------------------------------------- #

### Creazione LISTA ###

# Creo una variabile che utilizzero' come contenitore per la mia lista
var=
# Uso un ciclo per riempire la lista
for i; do
    # Inserisco il nuovo elemento in coda alla lista
    var="$var $i"
done

# -------------------------------------------------------------------------------------------------------------- #

### Richiesta di INPUT BOOLEANO ###

# Interrogo l'utente
echo "Domanda? [S/N]"
# Leggo la risposta dell'utente
read -r risposta
# Controllo la risposta dell'utente
case $risposta in
S*|s*|Y*|y*)
    ;;
N*|n*)
    # Non faccio nulla
    ;;
*)
# Risposta non valida
echo "Risposta non valida.";;
esac

# -------------------------------------------------------------------------------------------------------------- #

### Creazione FILE TEMPORANEO SENZA PID ###
### Eliminazione FILE TEMPORANEO ###

# Creo un file temporaneo
echo "Creo un file temporaneo."
> /tmp/nomiAssoluti



# Elimino il file temporaneo
echo "Elimino il file temporaneo."
rm /tmp/nomiAssoluti

# -------------------------------------------------------------------------------------------------------------- #

## Creazione di MULTIPLE FILE TEMPORANEI ##
## Eliminazione di MULTIPLE FILE TEMPORANEI ##

# Dichiaro una variabile da usare come contatore
n=1
# Scorro la lista in un ciclo che utilizza il contatore n
for G; do
    # Creo un file temporaneo per ogni gerarchia
    > /tmp/nomiAssoluti-$n

    # Incremento la variabile contatore di 1 a ogni iterazione
    n=`expr "$n" + 1`
done
# Reimposto il contatore al punto di partenza, ovvero a 1
n=1



# Elimino i file temporanei
# Dichiaro un contatore
n=1
for G; do
    # Elimino i file temporanei per ogni gerarchia
    rm /tmp/nomiAssoluti-$n

    # Incremento la variabile contatore di 1 a ogni iterazione
    n=`expr "$n" + 1`
done

# -------------------------------------------------------------------------------------------------------------- #