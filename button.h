#ifndef BUTTON_H
#define BUTTON_H

#define CLICK_DELAY 15

enum Button {
    KeyUp    = 0,
    KeyLeft  = 1,
    KeyRRight= 4,
    KeyRight = 2,
    KeyDown  = 3,
    KeyEnd   = 5
};

int checkButton(enum Button button);
void setupButtons(void);

#endif