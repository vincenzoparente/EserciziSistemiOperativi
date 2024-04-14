#!/bin/sh

# Mi sposto nella directory passata come primo parametro
cd "$1"

# Mi assicuro che la directory non sia vuota
if test "`ls -A`" = ""; then
    echo "Ho raggiunto la directory $1 che e' vuota. Directory scartata."
    # Esco con valore 0 perché cio' non e' un errore
    exit 0
fi

# Controllo che la directory contenga almeno un file con terminazione .$2
for F in *; do
    # Verifico se $F e' una directory e se e' traversabile
    if test -d "$F" -a -x "$F"; then
        # In tal caso chiamo lo script ricorsivo passandogli il path assoluto della directory
        FCR.sh "$1/$F" "$2" "$3"
    fi
done