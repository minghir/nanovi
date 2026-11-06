# Setări compilator și flag-uri
CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = vi
SRCS = vi.c nano_libc_host.c
OBJS = $(SRCS:.c=.o)
HEADERS = nano_libc.h

# Regula principală (default)
all: $(TARGET)

# Regula de link-editare pentru executabil
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Regula de compilare pentru fișierele obiect (.o)
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Regula pentru modul de debug (fără optimizări, cu simboluri de debug)
debug: CFLAGS += -g -DDEBUG -O0
debug: clean all

# Curățarea fișierelor generate
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all debug clean