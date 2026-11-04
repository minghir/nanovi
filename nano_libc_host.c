#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <termios.h>
#include <sys/stat.h>
#include "nano_libc.h"

// --- Terminal & Ecran ---

void nano_clear_screen(void) {
    printf("\033[H\033[J");
    fflush(stdout);
}

void nano_print(const char* str) {
    if (str) {
        fputs(str, stdout);
        fflush(stdout);
    }
}

// --- Citire tastatură în mod "Raw" ---

char nano_read_char(void) {
    struct termios oldt, newt;
    char ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

// --- Gestionare Fișiere ---

int nano_read_file(const char* name, uint8_t* buffer, uint32_t max_size) {
    FILE* f = fopen(name, "rb");
    if (!f) return -1;

    size_t bytes_read = fread(buffer, 1, max_size, f);
    fclose(f);
    return (int)bytes_read;
}

int nano_write_file(const char* name, const uint8_t* data, uint32_t size) {
    FILE* f = fopen(name, "wb");
    if (!f) return -1;

    size_t bytes_written = fwrite(data, 1, size, f);
    fclose(f);
    return (bytes_written == size) ? 0 : -1;
}

// Actualizat să accepte și argumentul de mod/permisiuni cerut de vi.c
int nano_create_file(const char* name, uint32_t size, int mode) {
    FILE* f = fopen(name, "wb");
    if (!f) {
        perror("[Host Error] fopen a eșuat");
        return -1;
    }

    if (size > 0) {
        int fd = fileno(f);
        if (ftruncate(fd, size) != 0) {
            perror("[Host Error] ftruncate a eșuat");
            fclose(f);
            return -1;
        }
    }
    
    fclose(f);
    chmod(name, mode); // Setează permisiunile primite (ex: 0644)
    return 0;
}

// --- Funcții noi cerute de erori ---

char* nano_strcat(char* dest, const char* src) {
    return strcat(dest, src);
}

int starts_with(const char* str, const char* pre) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

int nano_get_video_info(VideoModeInfo* info) {
    // Valori implicite standard pentru terminal (24 rânduri, 80 coloane)
    if (info) {
        info->rows = 24;
        info->cols = 80;
        return 1;
    }
    return 0;
}

void nano_draw_full_screen(const uint16_t* buffer) {
    if (buffer) {
        // Poți lăsa corpul gol momentan sau să procesezi bufferul uint16_t
        (void)buffer;
    }
}

void nano_set_cursor_shape(int shape) {
    // Secvențe ANSI pentru formă cursor (opțional, ignorat sau aplicat)
    (void)shape;
}

// --- Utilitare memorie / șiruri ---

int64_t parse_int64(const char* str) {
    int64_t res = 0;
    int sign = 1;
    int i = 0;
    
    if (str[0] == '-') {
        sign = -1;
        i = 1;
    } else if (str[0] == '+') {
        i = 1;
    }
    
    while (str[i] >= '0' && str[i] <= '9') {
        res = res * 10 + (str[i] - '0');
        i++;
    }
    
    return res * sign;
}

void* nano_malloc(uint32_t size) {
    return malloc(size);
}

void nano_free(void* ptr) {
    free(ptr);
}

int nano_read_file_host(const char* name, uint8_t* buffer, uint32_t max_size) {
    return nano_read_file(name, buffer, max_size);
}