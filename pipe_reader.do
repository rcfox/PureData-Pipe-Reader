for file in `find . -name '*.c' -print`; do
	DEPS="$DEPS ${file%.c}.o ";
done

redo-ifchange $DEPS

CFLAGS="-rdynamic -shared"
LIBS=""

gcc $CFLAGS $LIBS $DEPS -o $1.pd_linux
