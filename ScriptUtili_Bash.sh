#!/usr/bin/bash

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
PATH=$(pwd):$PATH
# Aggiungo le modifiche effettuate alla variabile d'ambiente esportandola
export PATH

# -------------------------------------------------------------------------------------------------------------- #

### Controllo PARAMETRI STRETTO ###

# Controllo che il numero di parametri sia esattamente quello desiderato
if [[ $# -ne 3 ]]; then
    # In tal caso il numero dei parametri e' diverso da quello richiesto, quindi interrompo l'esecuzione
    echo "Numero parametri errato. Processo interrotto."
    # Esco specificando un valore intero di errore
    exit 1
fi

# -------------------------------------------------------------------------------------------------------------- #

### Controllo PARAMETRI LASCO ###

# Controllo che il numero di parametri sia rientri nella soglia desiderata
if [[ $# -lt 3 ]]; then
    # In tal caso il numero dei parametri non riesntra nell'intervallo desiderato, quindi interrompo l'esecuzione
    echo "Numero parametri errato. Processo interrotto."
    # Esco specificando un valore intero di errore
    exit 1
fi

# -------------------------------------------------------------------------------------------------------------- #

### Verifico che il PATH sia ASSOLUTO ###
### Verifico che se la DIRECTORY e' ESISTENTE ###
### Verifico che se la DIRECTORY e' TRAVERSABILE ###

case $i in
/*)
    echo "Il parametro $i e' un path assoluto."
    # Verifico l'esistenza della directory
    if [[ -d $i ]]; then
        echo "Il parametro $i e' una directory."
        # Verifico che la directory sia traversabile
        if [[ -x $i ]]; then
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
    fi
    ;;
*)
    echo "Il parametro $i non e' un path assoluto. Processo interrotto."
    # Esco specificando un valore intero di errore
    exit 2
    ;;
esac

# -------------------------------------------------------------------------------------------------------------- #

### Ciclo con CONTATORE ###

# Scorro la lista parametri in un ciclo che utilizza il contatore i
for ((i = 1; i <= $#; i++)); do
    echo "$i"
done

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

### Controllo se il PARAMETRO e' un NUMERO INTERO ###
### Controllo se il PARAMETRO e' un NUMERO POSITIVO ###

# Controllo se il parametro e' un numero intero
case $i in
*[!0-9]*)
    # In tal caso il parametro non e' un numero intero, quindi interrompo l'esecuzione
    echo "Il parametro $i non e' un numero intero. Processo interrotto."
    # Esco specificando un valore intero di errore
    exit 5
    ;;
*)
    echo "Il parametro $i e' un numero intero."
    # Controllo se il numero e' positivo
    if [[ $i -le 0 ]]; then
        echo "Il numero $i non e' positivo."
        # Esco specificando un valore intero di errore
        exit 6
    else
        echo "Il numero $i e' positivo."
    fi
    ;;
esac

# -------------------------------------------------------------------------------------------------------------- #
