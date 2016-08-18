#include "menu.h"

#include <stdint.h>
#include <stddef.h>
#include "memfuncs.h"
#include "draw.h"
#include "hid.h"

int selected_options[MAX_SELECTED_OPTIONS];

int draw_menu(const char *title, int count, char *options[], const int *preselected, int *selected, enum menu_type type)
{
    if (count > MAX_SELECTED_OPTIONS) {
        return -1;
    }

    int current = 0;
    int pos_x_text = 0;
    int pos_y_text[count];
    int current_pos_y = 30;

    if (type == CHECKLIST)
    {
        memset(selected_options, 0, sizeof(selected_options));
        memset(selected, 0, sizeof(int) * count);
        pos_x_text = 4 * SPACING_HORIZ;
    }

    clear_screen(screen_top_left);
    draw_string(screen_top_left, title, 0, 0, COLOR_TITLE);

    pos_y_text[0] = current_pos_y;

    if (type == CHECKLIST)
        current_pos_y = draw_string(screen_top_left, "[ ]", 0, current_pos_y, COLOR_NEUTRAL);

    current_pos_y = draw_string(screen_top_left, options[0], pos_x_text, current_pos_y, COLOR_SELECTED);

    for (int i = 1; i < count; i++) {
        current_pos_y += SPACING_VERT;
        pos_y_text[i] = current_pos_y;

        if (type == CHECKLIST)
            draw_string(screen_top_left, "[ ]", 0, current_pos_y, COLOR_NEUTRAL);

        current_pos_y = draw_string(screen_top_left, options[i], pos_x_text, current_pos_y, COLOR_NEUTRAL);
    }

    if (type == CHECKLIST)
    {
        draw_string(screen_top_left, "Press START to confirm", 0, current_pos_y + SPACING_VERT * 2, COLOR_SELECTED);

        for (int i = 0; i < count; i++) {
            if (preselected[i]) {
                draw_character(screen_top_left, 'x', 0 + SPACING_HORIZ, pos_y_text[i], COLOR_NEUTRAL);
                selected_options[i] = 1;
            }
        }
    }

    while (1) {
        uint16_t key = wait_key();

        if (key == (key_released | key_up)) {
            draw_string(screen_top_left, options[current], pos_x_text, pos_y_text[current], COLOR_NEUTRAL);

            if (current <= 0) {
                current = count - 1;
            } else {
                current--;
            }

            draw_string(screen_top_left, options[current], pos_x_text, pos_y_text[current], COLOR_SELECTED);
        } else if (key == (key_released | key_down)) {
            draw_string(screen_top_left, options[current], pos_x_text, pos_y_text[current], COLOR_NEUTRAL);

            if (current >= count - 1) {
                current = 0;
            } else {
                current++;
            }

            draw_string(screen_top_left, options[current], pos_x_text, pos_y_text[current], COLOR_SELECTED);
        } else if (key == (key_released | key_a)) {
            if (type == CHECKLIST) {
                if (selected_options[current]) {
                    draw_character(screen_top_left, 'x', 0 + SPACING_HORIZ, pos_y_text[current], COLOR_BACKGROUND);
                    selected_options[current] = 0;
                } else {
                    draw_character(screen_top_left, 'x', 0 + SPACING_HORIZ, pos_y_text[current], COLOR_NEUTRAL);
                    selected_options[current] = 1;
                }
            }

            else if (type == SELECTION) {
                return current;
            }

        } else if (key == (key_released | key_start) || key == (key_released | key_b)) {
            if (type == CHECKLIST)
            {
                memcpy(selected, selected_options, sizeof(int) * count);
                return 0;
            }

            else if (type == SELECTION)
                return -1;
        }
    }
}

int draw_loading(const char *title, const char *text)
{
    clear_screen(screen_top_left);
    draw_string(screen_top_left, title, 0, 0, COLOR_TITLE);
    return draw_string(screen_top_left, text, 0, 30, COLOR_NEUTRAL);
}

void draw_message(const char *title, const char *text)
{
    int pos_y = draw_loading(title, text);

    draw_string(screen_top_left, "Press A to continue", 0, pos_y + 20, COLOR_SELECTED);

    while (1) {
        uint16_t key = wait_key();

        if (key == (key_released | key_a)) {
            return;
        }
    }
}
