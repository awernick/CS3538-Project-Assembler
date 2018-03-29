dist: assembler.c helpers.c utils.c
	cc -o assembler assembler.c helpers.c utils.c -lm

debug: assembler.c helpers.c utils.c
	cc -o assembler assembler.c helpers.c utils.c -lm -DDEBUG

clean:
	rm -f assembler
