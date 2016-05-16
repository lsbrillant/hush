OBJ = build/main.o build/makeargv.o \
	  build/command.o build/env.o \
	  build/opts.o build/hsignal.o

LIBDIR = /lib
LIB = $(LIBDIR)/x86_64-linux-gnu/libreadline.so.6

hush: main.o makeargv.o command.o env.o opts.o hsignal.o
	cc -o hush $(OBJ) $(LIB)

main.o: src/main.c  include/argv.h
	cc -c src/main.c -pthread -I include -o build/main.o

makeargv.o: src/makeargv.c include/argv.h
	cc -c src/makeargv.c -I include -o build/makeargv.o

env.o: src/env.c  include/env.h  include/argv.h
	cc -c src/env.c -I include -pthread -o build/env.o

command.o: src/command.c include/command.h  include/env.h include/argv.h
	cc -c src/command.c -I include -pthread -o build/command.o

opts.o: src/opts.c include/opts.h
	cc -c src/opts.c -I include -pthread -o build/opts.o

hsignal.o: src/signalsetup.c include/hushsignal.h
	cc -c src/signalsetup.c -I include -o build/hsignal.o

clean:
	rm build/main.o build/makeargv.o build/env.o build/command.o build/hsignal.o hush
