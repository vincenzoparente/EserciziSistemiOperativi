#!/bin/sh

# Mi sposto nella directory passata come primo parametro
cd "$1"

# Mi assicuro che la directory non sia vuota
if test "`ls -A`" = ""; then
    echo "Ho raggiunto la directory $1 che e' vuota. Directory scartata."
    # Esco con valore 0 perché cio' non e' un errore
    exit 0
fi

# Cerco directory da esplorare all'interno della directory corrente
for D in *; do
    if test -d "$D" -a -x "$D"; then
        # Chiamo lo script ricorsivo entrando nella directory
        FCR.sh "$1/$D" "$2" "$3"
    fi
done