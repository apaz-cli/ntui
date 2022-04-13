#ifndef NTUI_CURSES_INCLUDE
#define NTUI_CURSES_INCLUDE

#include <ncurses.h>
#include <sys/ioctl.h>

#include "tuiwin.h"

/* State */
typedef struct {
  WINDOW *curses_window;
  tuiwin_t root_window;
  int lastx, lasty;
} ntui_t;

static ntui_t ntui;

/* Canvas */
typedef struct {

} ntui_curses_canvas_t;
typedef ntui_curses_canvas_t ntui_canvas_t;

static inline void ntui_init(void) {

  /* Initialize ncurses. */
  ntui.curses_window = initscr();

  /* Set up the tty to be taken over. */
  raw();
  keypad(stdscr, TRUE);
  noecho();
  cbreak();
  clear();

  mousemask(ALL_MOUSE_EVENTS | BUTTON_SHIFT | BUTTON_CTRL | BUTTON_ALT |
                REPORT_MOUSE_POSITION,
            NULL);

  /* Ensure reporting mouse position works. */
  const char update_pos[] = "\033[?1003h\n";
  printf("%s", update_pos);
  fflush(stdout);

  /* Set up a the root window. */
  ntui_tuiwin_init(&ntui.root_window, NULL, 0, 0, 0, 0);
}

static inline void ntui_render(void) {}

static void ntui_input_loop() {
  int c, count = 0;
  MEVENT mouse_event;
  while ((c = getch())) {
    if (c == KEY_MOUSE) {
      if (getmouse(&mouse_event) == ERR)
        ERROR("Failed to recieve mouse event.");

      /* Mouse events bubble down. */

    } else if (c == KEY_RESIZE) {
      /* Resizes bubble down, each window is in charge of re-rendering itself onto the canvas. */
      ntui_render();
    } else {
      /* Keypresses bubble up, not down. */
    }
  }
}

static inline void ntui_destroy(void) { endwin(); }

#endif /* NTUI_CURSES_INCLUDE */
