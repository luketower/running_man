COMPILE = gcc -g -Wall main.cpp -I include -L lib -l SDL2-2.0.0 -l SDL2_image

build:
	$(COMPILE)

run:
	$(COMPILE) && ./a.out

clean:
	rm ./a.out && rm -rf a.out.DSYM
