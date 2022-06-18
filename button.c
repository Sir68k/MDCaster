#include "button.h"
#include "firmware.h"

static const enum KeyState key_map[] = { KeyPrev, KeyStop, KeyPlay, KeyNext, KeyPause, KeyEndS };
static uint32_t lastClick[] = {0, 0, 0, 0, 0, 0};

const volatile uint32_t* reset_ptr = (uint32_t*)0x02100000; // will cause data abort on read

int checkButton(enum Button button)
{
  int btn = (get_key_state() & 0xFE);

  if (btn == KeyMenu) // reset on menu press through data abort
    btn = *reset_ptr;
    
  int pressed = btn == key_map[button];
  uint32_t t = get_sys_ltime();
  if (pressed == 1 && (t - lastClick[button]) > CLICK_DELAY) {
    lastClick[button] = t;
    return 1;
  } 

  return 0;
}
