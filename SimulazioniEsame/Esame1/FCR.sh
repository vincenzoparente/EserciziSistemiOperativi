#!/usr/bin/bash

# Creo un file temporaneo
touch /tmp/mytemp
echo "File creato correttamente"

# Prendo la lista dei file della directory
ls "$1" >/tmp/mytemp
echo "Stampa eseguita correttamente"

# Varibile che segnala se ho trovato o no una sottodir (0 se non c'e', 1 altrimenti)
flag=0
echo "Flag e' $flag"
for i in $(cat /tmp/mytemp); do
    # Controllo se il file e' una directory
    if test -d "$i"; then
        echo "Sottodirectory trovata."
        flag=1
        echo "Flag e' $flag"
        # In tal caso avvio un ulteriore processo di esplorazione
        ./FCR.sh "$i" "$2"
    else
        if [[ $(wc -l) -le "$2" ]]; then
            echo "Il file ha lunghezza (in linee) inferirore o uguale a $2"
            flag=1
            echo "Flag e' $flag"
        fi
    fi
done

# Controllo se tra i file c'e' almeno una directory
if [[ "$flag" -eq 0 ]]; then
    echo "Directory valida."
    dirOnlyFile=$(("$dirOnlyFile" + 1))
fi
