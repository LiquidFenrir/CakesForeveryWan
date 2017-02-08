#pragma once

#define MAX_SELECTED_OPTIONS 0xFF

#define COLOR_TITLE 0x0000FF
#define COLOR_NEUTRAL 0xFFFFFF
#define COLOR_SELECTED 0xFF0000
#define COLOR_BACKGROUND 0x000000

enum menu_type {
    SELECTION,
    CHECKLIST
};

int draw_menu(const char *title, int count, char *options[], const int *preselected, int *selected, enum menu_type type);
int draw_loading(const char *title, const char *text);
void draw_message(const char *title, const char *text);
