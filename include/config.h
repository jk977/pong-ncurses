#ifndef CONFIG_H_
#define CONFIG_H_

#include <curses.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PONG_REFRESH_RATE   30 /* Hz */
#define PONG_PADDLE_HEIGHT  3  /* rows */

// minimum window dimension requirements
#define PONG_REQUIRED_X 50
#define PONG_REQUIRED_Y 20

#define PONG_COLOR_PAIR 1
#define PONG_BG         COLOR_BLUE
#define PONG_FG         COLOR_WHITE

#ifdef __cplusplus
}
#endif

#endif // CONFIG_H_
