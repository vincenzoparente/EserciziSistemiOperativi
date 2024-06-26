#!/bin/sh

# Controllo che il numero di parametri rientri nella soglia desiderata
if test $# -lt 4
then
    # In tal caso il numero dei parametri non riesntra nell'intervallo desiderato, quindi interrompo l'esecuzione
    echo "Numero parametri errato. Utilizzo corretto di $0: $0 numero_intero dir_ass1 dir_ass2."
    echo  "Processo interrotto."
    # Esco specificando un valore intero di errore
    exit 1;
fi



# Dichiaro una variabile lista per i path assoluti
Q=

# Scorro i parametri
for G; do
    # Uso un case per verificare se il parametro e' un path assoluto
    case $G in
    /*)
        echo "Il parametro $G e' un path assoluto."
        # Ora verifico l'esistenza della directory e che essa sia traversabile
        if test -d "$G" -a -x "$G"; then
            echo "Il parametro $G e' una directory traversabile."
            # Aggiorno la lista dei path assoluti
            Q="$Q $G"
        else
            echo "Il parametro $G non e' una directory traversabile. Processo interrotto."
            # Esco specificando un valore intero di errore
            exit 3
        fi;;
    *)
        echo "Il parametro $G non e' un path assoluto. Processo interrotto."
        # Esco specificando un valore intero di errore
        exit 2;;
    esac
done

# Aggiungo alla variabile d'ambiente PATH il path corrente
PATH=`pwd`:$PATH
# Aggiungo le modifiche effettuate alla variabile d'ambiente esportandola
export PATH

# Creo un file temporaneo
echo "Creo un file temporaneo."
> /tmp/nomiAssoluti

# Scorro la lista di gerarchie
for G in $Q; do
    # Chiamo lo script ricorsivo
    FCR.sh "$G" "$" "/tmp/nomiAssoluti"
done



# Elimino il file temporaneo
echo "Elimino il file temporaneo."
rm /tmp/nomiAssoluti
