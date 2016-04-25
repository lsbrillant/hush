hush: main.o makeargv.o command.o env.o
	cc -o hush build/main.o build/makeargv.o build/command.o build/env.o

main.o: src/main.c  include/argv.h
	cc -c src/main.c -pthread -I include -o build/main.o

makeargv.o: src/makeargv.c include/argv.h
	cc -c src/makeargv.c -I include -o build/makeargv.o

env.o: src/env.c  include/env.h  include/argv.h
	cc -c src/env.c -I include -pthread -o build/env.o

command.o: src/command.c include/command.h  include/env.h
	cc -c src/command.c -I include -pthread -o build/command.o

clean:
	rm build/main.o build/makeargv.o build/env.o build/command.o hush