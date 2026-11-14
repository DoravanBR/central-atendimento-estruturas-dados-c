CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic

TARGET = central.exe

SOURCES = \
	main.c \
	atendimento.c \
	entrada.c \
	repositorio.c \
	pilha.c \
	fila.c \
	bst.c \
	avl.c \
	heap.c \
	arvore_decisao.c \
	validacao.c \
	sistema.c \
	experimentos.c

OBJECTS = $(SOURCES:.c=.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(OBJECTS) $(TARGET)
