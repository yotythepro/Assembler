CC          = gcc 
CFLAGS      = -ansi -Wall -pedantic -lm -g
EXE_DEPS    =  main.o preprocessor.o macro.o firstPass.o validation.o parsing.o cmdList.o labels.o codeImage.o datalmage.o encoding.o testing.o secondPass.o output.o strOps.o

assembler:   $(EXE_DEPS)
	$(CC) $(EXE_DEPS) $(CFLAGS)  -o $@

%.o: %.c %.h Makefile
	$(CC) -c $< $(CFLAGS) -o $@
	
clean:
	rm -rf *.o
