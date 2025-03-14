
# Nome do executável
EXEC = teste

# Arquivo fonte
SRCS = teste.c

# Compilador
CC = gcc

# Flags do compilador
CFLAGS = -Wall -pthread

# Regra principal
all: $(EXEC)

# Regra para compilar o programa
$(EXEC): $(SRCS)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRCS)

# Regra para limpar arquivos gerados
clean:
	rm -f $(EXEC)

