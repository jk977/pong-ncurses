/*
 * config.h: Compile-time and runtime configuration for various game settings.
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <curses.h>
#include <signal.h>

#ifdef __cplusplus
extern "C" {
#endif

// minimum window dimension requirements
#define PONG_REQUIRED_X 50
#define PONG_REQUIRED_Y 20

// color settings
#define PONG_MAIN_COLOR 1
#define PONG_MAIN_BG    COLOR_BLUE
#define PONG_MAIN_FG    COLOR_WHITE

// misc settings
#define PONG_PADDLE_HEIGHT 4  /* rows */
#define PONG_PLAYER_MAX    2
#define PONG_WIN_SCORE     10
#define PONG_OUTER_BUFFER  5  /* rows/cols */

#define PONG_EASY_HZ    15
#define PONG_HARD_HZ    30

#define PONG_RESTART_SIG    SIGUSR1
#define PONG_PAUSE_SIG      SIGUSR2

// runtime settings
extern unsigned int PONG_REFRESH_RATE; /* Hz */

#ifdef __cplusplus
}
#endif

#endif // CONFIG_H_
