CC=musl-gcc
#CC=gcc

EXEC=example

HEADERS=\
	src/tralloc.h \
	src/trs.h     \
	src/trs_vec.h \

SRC=\
    src/example.c \
    src/tralloc.c \
    src/trs.c     \
    src/trs_vec.c \

OBJS=$(SRC:.c=.o)

# path for the 'utils/' headers
INCLUDE=\
	-I/usr/local/include/ \

CFLAGS=\
       $(INCLUDE)   \
       -Wall        \
       -Wconversion \
       -Wextra      \
       -std=c11     \

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

clean:
	$(RM) $(OBJS) $(EXEC)

cppcheck:
	cppcheck -DVEC_DTOR=_trs_free --language=c --enable=all --inconclusive -f -v $(INCLUDE) $(SRC) $(HEADERS)
