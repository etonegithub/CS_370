CC=gcc 
FILES=profiler.c 
OUT_EXE=profiler
all: $(FILES) 
	$(CC) -o $(OUT_EXE) $(FILES) 
clean: 
	rm -f *.o *.zip $(OUT_EXE) 
package:
	zip -r Evan_Tone_ExtraCredit.zip $(FILES) Makefile README.txt