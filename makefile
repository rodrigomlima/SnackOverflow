# Arquivo Makefile para compilar o programa de ordenação de vetores com multithreads
#
# Compilador padrao
CC=gcc
#
# Arquivos fonte
FONTES=projSO.c arquivo.c mergesort.c
OBJETOS=$(FONTES:.c=.o)
EXECUTAVEL=projSO

#Dependencias de Compilacao
all: $(EXECUTAVEL)

projSO: arquivo.o mergesort.o
	$(CC) -o projSO projSO.c arquivo.o mergesort.o -lpthread

arquivo.o: arquivo.c arquivo.h
	$(CC) -c arquivo.c

mergesort.o: mergesort.c mergesort.h
	$(CC) -c mergesort.c

#Limpeza
clean:
	rm -f $(OBJETOS) $(EXECUTAVEL)
