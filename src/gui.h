#ifndef GUI_H
#define GUI_H

struct program_state;

static void debug_text_bool(const char *text, const bool *condition);

void debug_window_draw(program_state *program);

void side_bar_draw(program_state *program);

#endif