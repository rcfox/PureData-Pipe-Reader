redo-ifchange $1.c

CFLAGS="-fPIC"
INCLUDE=""

gcc $CFLAGS $INCLUDE -MD -MF $1.d -c -o $3 $1.c

read DEPS <$1.d
redo-ifchange ${DEPS#*:}
rm $1.d
