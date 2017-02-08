#include "menu.h"

#include <stdint.h>
#include <stddef.h>
#include "memfuncs.h"
#include "draw.h"
#include "hid.h"

#define MAX_OPTIONS_PER_SCREEN 0x10

int selected_options[MAX_SELECTED_OPTIONS];
static int scroll = 0;

static void draw_menu_sub(const char *title, int count, char *options[], int *selected, enum menu_type type, int change, int current)
{
	if (change != 0)
	{
		if (current == 0) {
			scroll = 0;
		}
		else if(current == count - 1) {
			scroll = count - MAX_OPTIONS_PER_SCREEN;
			if (scroll < 0)
				scroll = 0;
		}
		else if (current >= (MAX_OPTIONS_PER_SCREEN + scroll)) {
			scroll++;
		}
		else if ((current - scroll) < 0) {
			scroll--;
		}
		
    int pos_x_text = 0;
    int pos_y_text[count];
    int current_pos_y = 30;

    if (type == CHECKLIST)
        pos_x_text = 4 * SPACING_HORIZ;

    clear_screen(screen_top_left);
    draw_string(screen_top_left, title, 0, 0, COLOR_TITLE);

    pos_y_text[scroll] = current_pos_y;

    current_pos_y = draw_string(screen_top_left, options[scroll], pos_x_text, current_pos_y, COLOR_SELECTED);

    for (int i = scroll; i < MAX_OPTIONS_PER_SCREEN+scroll; i++) {
				if (i >= count)
					break;
				
        pos_y_text[i] = current_pos_y;

        if (type == CHECKLIST)
            draw_string(screen_top_left, selected_options[i] ? "[x]" : "[ ]", 0, current_pos_y, COLOR_NEUTRAL);

        current_pos_y = draw_string(screen_top_left, options[i], pos_x_text, current_pos_y, (current == i) ? COLOR_SELECTED : COLOR_NEUTRAL);
				current_pos_y += SPACING_VERT;
    }
		
		if (type == CHECKLIST)
        draw_string(screen_top_left, "Press START to confirm", 0, current_pos_y + SPACING_VERT * 2, COLOR_SELECTED);
		
	}
}

int draw_menu(const char *title, int count, char *options[], const int *preselected, int *selected, enum menu_type type)
{
		scroll = 0;
		int change = 1;
		int current = 0;
		
		if (type == CHECKLIST)
		{
				memset(selected_options, 0, sizeof(selected_options));
        memset(selected, 0, sizeof(int) * count);
				for (int i = 0; i < count; i++) {
					selected_options[i] = preselected[i];
				}
		}
				
    while (1) {
				
				draw_menu_sub(title, count, options, selected, type, change, current);
				change = 0;
				
        uint16_t key = wait_key();

        if (key == (key_released | key_up)) {

            if (current <= 0) {
                current = count - 1;
            } else {
                current--;
            }
						
						change = 1;

        } else if (key == (key_released | key_down)) {

            if (current >= count - 1) {
                current = 0;
            } else {
                current++;
            }
						
						change = 1;

        } else if (key == (key_released | key_a)) {
            if (type == CHECKLIST) {
                selected_options[current] ^= 1;
            }

            else if (type == SELECTION) {
                return current;
            }
						
						change = 1;

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
