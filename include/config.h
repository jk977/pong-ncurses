#ifndef CONFIG_H_
#define CONFIG_H_

#include <curses.h>

#ifdef __cplusplus
extern "C" {
#endif

// default icons for game elements
#define PONG_DEFAULT_BALL   '*'
#define PONG_DEFAULT_WALL   '#'
#define PONG_DEFAULT_PADDLE '|'

// minimum window dimension requirements
#define PONG_REQUIRED_X 50
#define PONG_REQUIRED_Y 20

// color settings
#define PONG_MAIN_COLOR 1
#define PONG_MAIN_BG    COLOR_BLUE
#define PONG_MAIN_FG    COLOR_WHITE

// misc settings
#define PONG_REFRESH_RATE   30 /* Hz */
#define PONG_PADDLE_HEIGHT  3  /* rows */
#define PONG_PLAYER_MAX    2
#define PONG_OUTER_BUFFER  5 /* rows/cols */

#ifdef __cplusplus
}
#endif

#endif // CONFIG_H_
