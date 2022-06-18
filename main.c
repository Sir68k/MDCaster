#include "firmware.h"
#include "display.h"
#include "button.h"
#include "raytracer.h"

static int counter, refresh;
static uint8_t show_mini, toggle;

void main() {
    // kill all the tasks that we don't need
    for (int i=0; i<8; i++)
        tron_ter_task(i);

    setup_display();
    tron_dly_tsk(10);
    setup_display();
    tron_dly_tsk(10);

    setup_rays();
    cast_rays(0);

    draw_minimap(1);

    counter = 0;
    toggle  = 1;
    show_mini = 1;

    while (1) {
        int changed = 0;
        counter++;

        if (counter > 35) {
            counter = 0;
            toggle ^= 1;
        }
        
        if (checkButton(KeyUp))
            changed = player_move(1, 0, 0, 0);
        else if (checkButton(KeyDown))
            changed = player_move(0, 1, 0, 0);
        else if (checkButton(KeyLeft))
            changed = player_move(0, 0, 0, 1);
        else if (checkButton(KeyRight) || checkButton(KeyRRight))
            changed = player_move(0, 0, 1, 0);
        else if (checkButton(KeyEnd)) {
            show_mini = !show_mini;
            changed = 1;
        }            

        if (changed || counter == 0) {
            cast_rays();
            if (show_mini)
                draw_minimap(toggle & 1);
        }

        
        update_display();
        tron_dly_tsk(2);
    }
}
