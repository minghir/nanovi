#ifndef NANO_LIBC_H
#define NANO_LIBC_H

//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>


int sprintf(char* str, const char* format, ...);


typedef struct {
    char magic[4];       // "NAS1"
    uint32_t entry_offset;
} __attribute__((packed)) NanoHeader;

// Structură pentru modul video (compatibilitate vi.c)
typedef struct {
    int rows;
    int cols;
} VideoModeInfo;

// Prototypes pentru funcțiile nano folosite de nanovi
void nano_clear_screen(void);
void nano_print(const char* str);
char nano_read_char(void);
int nano_read_file(const char* name, uint8_t* buffer, uint32_t max_size);
int nano_write_file(const char* name, const uint8_t* data, uint32_t size);
int nano_create_file(const char* name, uint32_t size, int mode); // Actualizat cu permisiuni

// Helper pentru itoa
static inline void itoa(int n, char* buf, int radix) {
    if (radix == 10) {
        sprintf(buf, "%d", n);
    } else {
        sprintf(buf, "%x", n);
    }
}

int64_t parse_int64(const char* str);
void* nano_malloc(uint32_t size);
void nano_free(void* ptr);

// Funcții noi cerute de vi.c
int nano_get_video_info(VideoModeInfo* info);
void nano_draw_full_screen(const uint16_t* buffer);
void nano_set_cursor_shape(int shape);
char* nano_strcat(char* dest, const char* src);
int starts_with(const char* pre, const char* str);

int nano_read_file_host(const char* name, uint8_t* buffer, uint32_t max_size);

#endif