#!/usr/bin/sh

### Aggiornamento del PATH ###

# Aggiungo alla variabile d'ambiente PATH il path corrente
PATH=`pwd`:$PATH
# Aggiungo le modifiche effettuate alla variabile d'ambiente esportandola
export PATH



### Controllo PARAMETRI STRETTO ###

# Controllo che il numero di parametri sia esattamente quello desiderato
if test $# -ne )
then
    # In tal caso il numero dei parametri e' diverso da quello richiesto, quindi interrompo l'esecuzione
    echo "Numero parametri errato."
    exit 1;
fi



### Controllo PARAMETRI LASCO ###

# Controllo che il numero di parametri sia rientri nella soglia desiderata
if test $# -lt )
then
    # In tal caso il numero dei parametri e' minore di quello richiesto, quindi interrompo l'esecuzione
    echo "Numero parametri errato."
    exit 1;
fi



### Verifico che il PATH sia ASSOLUTO ###
### Verifico che se la DIRECTORY e' ESISTENTE ###

case $i in
/*)
    echo "Il parametro $i e' un path assoluto."
    if test -d $i
    then
        echo "Il parametro $i e' una directory."
    else
        echo "Il parametro $i non e' una directory."
        exit 3
    fi;;
*)
    echo "Il parametro $i non e' un path assoluto."
    exit 2;;
esac



### Ciclo con CONTATORE ###

# Dichiaro una variabile da usare come contatore
J=1
# Scorro la lista in un ciclo che utilizza il contatore J
for i
do
    # Incremento la variabile contatore di 1 a ogni iterazione
    J=`expr $J + 1`
done
# Reimposto il contatore al punto di partenza, ovvero a 1
J=1



### Creazione LISTA ###

# Creo una variabile che utilizzerò come contenitore per la mia lista
var=
# Uso un ciclo per riempire la lista
for i
do
    # Inserisco il nuovo elemento in coda alla lista
    var="$var $i"
done



# IS_DIR


# DIR_TRAVERSABILE
if test -x $i
then
    echo "Il parametro $i e' una directory traversabile."
else
    echo "Il parametro $i non e' una directory traversabile."
    exit 3
fi

# CONTROLLO COMBINATO
if test -d $i -a -x $i
then
    echo "Il parametro $i e' una directory."
else
    echo "Il parametro $i non e' una directory."
    exit 3
fi

# INTERO
case $i in
*[!0-9]*)
    echo "Il parametro $i non e' un numero intero."
    exit 2;;
*)
    echo "Il parametro $i e' un numero intero."
esac

# POSITIVO
if test `expr $i` -le 0
then
    echo "Il numero $i non e' positivo."
else
    echo "Il numero $i e' positivo."
fi