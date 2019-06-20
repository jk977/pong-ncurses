#ifndef MENU_H_
#define MENU_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct settings {
    bool is_multiplayer;

    enum {EASY, HARD}
    difficulty;
};

struct settings menu_show(void);

#ifdef __cplusplus
}
#endif

#endif // MENU_H_
