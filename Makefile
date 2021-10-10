all:snake

snake: snake-stub.cpp terminalfunc.cpp terminalfunc.h
	g++ snake-stub.cpp terminalfunc.cpp -I. -o app

run: 
	./app

build-run: all run

clean: 
	rm app