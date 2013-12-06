TARGETS=util.o crypt.o userActions.o main.o
all: ${TARGETS} passwordKeeper

%.o: %.cpp
	g++ -c -o $@ $<

passwordKeeper: ${TARGETS}
	g++ ${TARGETS} -o passwordKeeper

clean:
	rm -f ${TARGETS} passwordKeeper
