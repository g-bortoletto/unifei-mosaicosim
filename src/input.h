#ifndef INPUT_H
#define INPUT_H

struct sapp_event;
struct program_state;

void update_mouse_position(const sapp_event *e, program_state *program);

void update_left_mouse_button(const sapp_event *e, program_state *program);

void update_right_mouse_button(const sapp_event *e, program_state *program);

void update_middle_mouse_button(const sapp_event *e, program_state *program);

#endif