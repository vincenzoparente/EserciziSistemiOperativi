#!/usr/bin/sh

# CONTATORE
N=1
for i
do
    N=`expr $N + 1`
done
# reset a 1
N=1

# AGGIORNAMENTO PATH
PATH=`pwd`:$PATH
export PATH

# LISTA
var=
for i
do
    var="$var $i"
done

# STRETTO
if test $# -ne 3
then
    echo "Numero parametri errato."
    exit 1;
fi

# LASCO
if test $# -lt 3
then
    echo "Numero parametri errato."
    exit 1;
fi

# PATH
case $i in
/*)
    echo "Il parametro $i e' un path assoluto.";;
*)
    echo "Il parametro $i non e' un path assoluto."
    exit 2;;
esac

# IS_DIR
if test -d $i
then
    echo "Il parametro $i e' una directory."
else
    echo "Il parametro $i non e' una directory."
    exit 3
fi

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