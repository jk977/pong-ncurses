#ifndef SCREENS_H_
#define SCREENS_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

int screen_start(unsigned int* refresh_rate, bool* is_multiplayer);
int screen_pause(void);
int screen_win(char const* player);

#ifdef __cplusplus
}
#endif

#endif // SCREENS_H_
