#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include "nano_libc.h"

static struct termios orig_termios;

// --- Gestionare Terminal Raw ---

static void disable_raw_mode(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    // Reactivăm auto-wrap/scroll și afișăm cursorul normal la ieșire
    printf("\033[?7h\033[?25h\033[0m\n");
    fflush(stdout);
}

static void enable_raw_mode(void) {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    raw.c_iflag &= ~(IXON | ICRNL);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void nano_clear_screen(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}

void nano_print(const char* str) {
    if (str) {
        fputs(str, stdout);
        fflush(stdout);
    }
}

// Citire tastatură cu suport complet pentru tasta Insert și Săgeți
char nano_read_char(void) {
    static int initialized = 0;
    if (!initialized) {
        enable_raw_mode();
        initialized = 1;
    }

    char c;
    if (read(STDIN_FILENO, &c, 1) <= 0) return 0;

    // Parser pentru Secvențe Escape de pe Linux
    if (c == 27) { // ESC
        char seq[4];
        struct timeval tv = {0, 50000}; // 50ms timeout
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);

        if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0) {
            if (read(STDIN_FILENO, &seq[0], 1) <= 0) return 27;
            if (read(STDIN_FILENO, &seq[1], 1) <= 0) return 27;

            if (seq[0] == '[') {
                if (seq[1] >= '0' && seq[1] <= '9') {
                    if (read(STDIN_FILENO, &seq[2], 1) > 0 && seq[2] == '~') {
                        switch (seq[1]) {
                            case '2': return 137; // Tasta Insert -> Intrare în INSERT MODE
                            case '3': return 132; // Delete
                            case '5': return 134; // Page Up
                            case '6': return 136; // Page Down
                        }
                    }
                } else {
                    switch (seq[1]) {
                        case 'A': return 128; // SUS
                        case 'B': return 129; // JOS
                        case 'C': return 130; // DREAPTA
                        case 'D': return 131; // STÂNGA
                        case 'H': return 133; // HOME
                        case 'F': return 135; // END
                    }
                }
            }
        }
        return 27; // Tasta ESC apăsată singură
    }

    return c;
}

// --- Randare Ecran pe Linux ---

void nano_draw_full_screen(const uint16_t* buffer) {
    if (!buffer) return;

    VideoModeInfo vinfo;
    nano_get_video_info(&vinfo);
    int cols = vinfo.cols > 0 ? vinfo.cols : 80;
    int rows = vinfo.rows > 0 ? vinfo.rows : 24;

    // Mutăm cursorul sus în colțul (1,1)
    printf("\033[H");

    uint16_t last_attr = 0xFFFF;
    int total_cells = cols * rows;

    for (int i = 0; i < total_cells; i++) {
        uint16_t cell = buffer[i];
        uint16_t attr = cell & 0xFF00;
        char ch = (char)(cell & 0x00FF);

        if (attr != last_attr) {
            if (attr == 0x0B00 || attr == 0x7000) {
                // Inversare culori pentru linia de status / modul Visual
                printf("\033[7m");
            } else {
                // Reset la stil normal
                printf("\033[0m");
            }
            last_attr = attr;
        }

        putchar(ch);
    }
    
    printf("\033[0m");
    fflush(stdout);
}

int nano_get_video_info(VideoModeInfo* info) {
    if (info) {
        struct winsize w;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_row > 0) {
            info->rows = w.ws_row;
            info->cols = w.ws_col;
        } else {
            info->rows = 24;
            info->cols = 80;
        }
        return 1;
    }
    return 0;
}

void nano_set_cursor_shape(int shape) {
    if (shape == 1) {
        // Cursor tip linie verticală pentru modul Insert
        printf("\033[6 \x71");
    } else {
        // Cursor tip bloc pentru modul Normal
        printf("\033[2 \x71");
    }
    fflush(stdout);
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
    return (bytes_written == size) ? (int)bytes_written : -1;
}

int nano_create_file(const char* name, uint32_t size, int mode) {
    FILE* f = fopen(name, "wb");
    if (!f) return -1;
    fclose(f);
    chmod(name, mode);
    return 0;
}

// --- Utilitare ---

char* nano_strcat(char* dest, const char* src) {
    return strcat(dest, src);
}

int starts_with(const char* pre, const char* str) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

int64_t parse_int64(const char* str) {
    return strtoll(str, NULL, 10);
}

void* nano_malloc(uint32_t size) {
    return malloc(size);
}

void nano_free(void* ptr) {
    if (ptr) free(ptr);
}

int nano_read_file_host(const char* name, uint8_t* buffer, uint32_t max_size) {
    return nano_read_file(name, buffer, max_size);
}