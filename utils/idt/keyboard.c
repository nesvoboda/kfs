#include "keyboard.h"


void handle_scroll(enum direction dir)
{
    screen_handle_scroll(dir, current_screen);
}

void erase()
{
  screen_erase(current_screen);
}

void next_screen()
{
  if (++current_screen == SCREEN_MAX)
    current_screen = 0;

  refresh_screen(current_screen);
}

void switch_color(int color)
{
  current_color = color;
}

void add_char(char c)
{
  screen_add_char(c, current_color, current_screen);
}

void keyboard_handler(registers_t regs)
{
  
	char scancode;
	scancode = inb(KBD_DATA_REG);

  if ((scancode & KBD_STATUS_MASK) == 0)
  {

    if (keyboard_flags.arrow_flag && scancode == UP_PRESSED)
      handle_scroll(TOP);
    else if (keyboard_flags.arrow_flag && scancode == BOTTOM_PRESSED)
      handle_scroll(BOTTOM);
    else if (keyboard_flags.arrow_flag && scancode ==  LEFT_PRESSED)
      handle_scroll(LEFT);
    else if (keyboard_flags.arrow_flag && scancode == RIGHT_PRESSED)
      handle_scroll(RIGHT);
    else if (scancode == ALT)
      keyboard_flags.alt_flag = 1;
    else if (scancode == TAB && keyboard_flags.alt_flag)
      next_screen();
    else if ((scancode >= 2 && scancode <= 9) && keyboard_flags.alt_flag)
      switch_color(scancode - 1);
    else if (scancode == BACKSPAPCE)
      erase();
    else if (scancode != ARROW && scancode != LEFT_PRESSED  && scancode != BOTTOM_PRESSED  && scancode != UP_PRESSED  && scancode != RIGHT_PRESSED) {
      char c = kbdus[scancode & KBD_SCANCODE_MASK];
      if (c > 32 & c <= 127) {
        add_char(c);
      }
    }
      
  }
  else   
  {
    if (scancode == ALT_REALEASE)
      keyboard_flags.alt_flag = 0;
    if (scancode == ARROW)
      keyboard_flags.arrow_flag = !keyboard_flags.arrow_flag;
  }
}