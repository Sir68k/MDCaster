#include <stdio.h>
#include <stdint.h>

#include "firmware.h"
#include "display.h"
#include "button.h"

#define bool uint8_t
#define true 1
#define false 0

int16_t mul(int16_t a, int16_t b) {
    return (int16_t)(((int32_t)a * (int32_t)b) >> 8)  ;
}

struct deltaDistT {
    int16_t x;
    int16_t y;
};

struct LUT_Data {
    int16_t xDir;
    int16_t yDir;
    struct deltaDistT deltas[7];

    int16_t align;
};


struct LUT_Data LUT[] = {{256,0,{{256,-1536},{256,-2304},{256,-4608},{256,16383},{256,4607},{256,2303},{256,1536}},0},{247,-67,{{277,-610},{273,-700},{269,-820},{265,-990},{261,-1248},{257,-1690},{253,-2617}},0},{221,-128,{{327,-398},{315,-430},{305,-468},{295,-513},{286,-567},{277,-635},{269,-720}},0},{181,-182,{{434,-311},{407,-326},{383,-343},{362,-363},{342,-384},{325,-408},{310,-435}},0},{128,-222,{{719,-270},{634,-278},{566,-287},{511,-296},{467,-306},{429,-316},{397,-328}},0},{66,-248,{{2616,-254},{1689,-258},{1247,-262},{989,-266},{819,-270},{699,-274},{609,-278}},0},{0,-256,{{-1537,-256},{-2305,-256},{-4609,-256},{16383,-256},{4607,-256},{2303,-256},{1535,-256}},0},{-67,-248,{{-610,-278},{-700,-274},{-820,-270},{-990,-266},{-1248,-262},{-1690,-258},{-2617,-254}},0},{-128,-222,{{-398,-328},{-430,-316},{-468,-306},{-513,-296},{-567,-287},{-635,-278},{-720,-270}},0},{-182,-182,{{-311,-435},{-326,-408},{-343,-384},{-363,-363},{-384,-343},{-408,-326},{-435,-311}},0},{-222,-129,{{-270,-720},{-278,-635},{-287,-567},{-296,-512},{-306,-468},{-316,-430},{-328,-398}},0},{-248,-67,{{-254,-2617},{-258,-1690},{-262,-1248},{-266,-990},{-270,-820},{-274,-700},{-278,-610}},0},{-257,-1,{{-256,1536},{-256,2304},{-256,4608},{-256,-16383},{-256,-4608},{-256,-2304},{-256,-1536}},0},{-248,66,{{-278,609},{-274,699},{-270,819},{-266,989},{-262,1247},{-258,1689},{-254,2616}},0},{-222,127,{{-328,397},{-316,429},{-306,467},{-296,512},{-287,566},{-278,634},{-270,719}},0},{-182,181,{{-435,310},{-408,325},{-384,342},{-363,362},{-343,383},{-326,407},{-311,434}},0},{-129,221,{{-720,269},{-635,277},{-567,286},{-512,295},{-468,305},{-430,315},{-398,327}},0},{-67,247,{{-2617,253},{-1690,257},{-1248,261},{-990,265},{-820,269},{-700,273},{-610,277}},0},{-1,256,{{1536,255},{2304,255},{4608,255},{-16383,255},{-4608,255},{-2304,255},{-1536,255}},0},{66,247,{{609,277},{699,273},{819,269},{989,265},{1247,261},{1689,257},{2616,253}},0},{127,221,{{397,327},{429,315},{467,305},{512,295},{566,286},{634,277},{719,269}},0},{181,181,{{310,434},{325,407},{342,383},{362,362},{383,342},{407,325},{434,310}},0},{221,128,{{269,719},{277,634},{286,566},{295,511},{305,467},{315,429},{327,397}},0},{247,66,{{253,2616},{257,1689},{261,1247},{265,989},{269,819},{273,699},{277,609}},0}};
uint8_t height_lut[] = {0,240,120,80,60,48,40,34,30,26,24,21,20,18,17,16,15,14,13,12,12,11,10,10,10,9,9,8,8,8,8,7,7,7,7,6,6,6,6,6,6,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};
struct LUT_Data *current;

#define VAL_1 (256)
#define SCREEN_WIDTH 7
#define SCREEN_HEIGHT 30

#define WH SCREEN_HEIGHT*SCALE
#define WW SCREEN_WIDTH*SCALE

int16_t xPos;//      = ;//(int16_t)(4.5f * 256);
int16_t yPos;//      = 1152;//(int16_t)(4.5f * 256);
int16_t moveSpeed;// = 128;//(int16_t)(0.5f * 256);
int16_t xDir, yDir;

#define mapWidth 14
#define mapHeight 14


int16_t xEnemy = 10;
int16_t yEnemy = 10;
int16_t zEnemy = 0;

const uint8_t level_map[mapHeight][mapWidth] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1},
    {1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1},
    {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void drawStraightLine(int x, int drawStart, int drawEnd, int v, int l) {
    for (int y=drawStart; y<(drawEnd & 31); y++)
        set_pixel(y, x, v, l); //swap x y
}

void draw_minimap(int toggle, int r) {
    int32_t mapX, mapY, offsetX, offsetY;
    mapX = xPos >> 8;
    mapY = yPos >> 8;

    offsetX = mapX - CHAR_W  / 2;
    offsetY = mapY - CHAR_H / 2;

    if (offsetX < 0)
        offsetX = 0;
    if (offsetY < 0)
        offsetY = 0;

    if (offsetX + CHAR_W > mapWidth)
        offsetX = (mapWidth - CHAR_W);
    if (offsetY + CHAR_H > mapHeight)
        offsetY = (mapHeight - CHAR_H);
    
    for (int y=0; y<CHAR_H; y++) {
        for (int x=0; x<CHAR_W; x++) {
            set_pixel(CHAR_W*6 - x - 1, y, level_map[offsetY+y][offsetX+x]&1, 0); 
            set_pixel(CHAR_W*6 - x - 1, y, level_map[offsetY+y][offsetX+x]&1, 1); 

            if (offsetY+y == yEnemy && offsetX+x == xEnemy) {
                set_pixel(CHAR_W*6 - x - 1, y, 0, 0); 
                set_pixel(CHAR_W*6 - x - 1, y, 1-toggle, 1); 
            }
        }
    }

    set_pixel(CHAR_W*5 + mapX - offsetX, mapY - offsetY, toggle, 0); 
    set_pixel(CHAR_W*5 + mapX - offsetX, mapY - offsetY, toggle, 1);
}


void cast_rays() {
    int16_t sideDistX, sideDistY, deltaDistX, deltaDistY, perpWallDist;
    int16_t mapX, mapY, stepX, stepY, side;

    int16_t enemy_dst = -1;
    zEnemy = (zEnemy + 1) % 16;
    
    int hit;

    for (int x = 0; x < SCREEN_WIDTH; x++) {
        mapX = xPos & 0xFF00;
        mapY = yPos & 0xFF00;
        
        deltaDistX = current->deltas[x].x;
        deltaDistY = current->deltas[x].y;

        side = 0;
        hit = false;

        if (deltaDistX < 0) {
            deltaDistX = -deltaDistX;
            stepX = -VAL_1;
            sideDistX = mul((xPos - mapX), deltaDistX);
        } else {
            stepX = VAL_1;
            sideDistX = mul((mapX + VAL_1 - xPos), deltaDistX);
        }

        if (deltaDistY < 0) {
            deltaDistY = -deltaDistY;
            stepY = -VAL_1;
            sideDistY = mul((yPos - mapY), deltaDistY);
        } else {
            stepY = VAL_1;
            sideDistY = mul((mapY + VAL_1 - yPos), deltaDistY);
        }

        while (!hit) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            if (level_map[mapY>>8][mapX>>8] == 1) {
                hit = true;
            } 
            
            /*
            else if ((mapY>>8) == yEnemy && (mapX>>8) == xEnemy) {
                if (side == 0)
                    enemy_dst = (sideDistX - deltaDistX)>>8;
                else
                    enemy_dst = (sideDistY - deltaDistY)>>8;
            }*/
        }

        if (side == 0) {
            perpWallDist = sideDistX - deltaDistX;
        } else {
            perpWallDist = sideDistY - deltaDistY;
        }

    
        int lineHeight;
        if (perpWallDist > 0) {
            if (perpWallDist>>5 > 80)
                lineHeight = 2;
            else
                 //lineHeight = abs(divv(SCREEN_HEIGHT, (perpWallDist)));
                lineHeight = height_lut[perpWallDist>>5];//(0x1E00>>5) / (perpWallDist>>5);
        } else
            lineHeight = SCREEN_HEIGHT;

        if (lineHeight <= 2)
            lineHeight= SCREEN_HEIGHT;


        // Calculate lowest and heighest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;

        if (drawStart < 0)
            drawStart = 0;

        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT)
            drawEnd = SCREEN_HEIGHT - 1;

        //console.log(mapY>>8,mapX>>8)
        int texNum = level_map[mapY>>8][mapX>>8];

        if (drawEnd >= SCREEN_HEIGHT)
            drawEnd = SCREEN_HEIGHT - 1;

        if (drawStart < 0)
            drawStart = 0;

        drawStraightLine(x, 0, SCREEN_HEIGHT, 0, 0);
        drawStraightLine(x, 0, SCREEN_HEIGHT, 0, 1);

        if (texNum == 1) {
            //drawStraightLine(x, 10, 23, 0, 1);
            drawStraightLine(x, drawStart, drawEnd+1, 1, 0);    
            drawStraightLine(x, drawStart, drawEnd+1, 1, side);          
        }  

        /*
        if (enemy_dst >= 0 && enemy_dst < 8 && x > 2 && x < 5) {
            uint8_t enemy_h = (8-enemy_dst);

            int drawStart = ((SCREEN_HEIGHT - enemy_h) / 2);

            drawStraightLine(x, drawStart, drawStart + enemy_h, zEnemy&1, 0);
            drawStraightLine(x, drawStart, drawStart + enemy_h, zEnemy&1, 1);

        }*/
    }
}

int move_idx = 0;
#define lut_len 24

int player_move(int forward,int backward,int left,int right) {
    int success = 0;
    int m,mm;

    if (forward) {
        m = mul(xDir, moveSpeed);
        mm = (xPos + m) >> 8;
        if (level_map[yPos>>8][mm] == 0
            && mm < mapWidth
            && mm > 0) {
            xPos += m;
            success = true;
        }
        
        m = mul(yDir, moveSpeed);
        mm = (yPos + m) >> 8;
        if (level_map[mm][xPos>>8] == 0
            && mm < mapWidth
            && mm > 0) {
            yPos += m;
            success = true;
        }
    } else if (backward) {
        m = mul(xDir, moveSpeed);
        mm = (xPos - m) >> 8;

        if (level_map[yPos>>8][mm] == 0
            && mm < mapWidth
            && mm > 0) {
            xPos -= m;
            success = true;
        }

        m = mul(yDir, moveSpeed);
        mm = (yPos - m) >> 8;
        if (level_map[mm][xPos>>8] == 0
            && mm < mapWidth
            && mm > 0) {
            yPos -= m;
            success = true;
        }
    }

    if (left) {
        move_idx = (move_idx + 1) % lut_len;
        current = &LUT[move_idx];

        xDir    = current->xDir;
        yDir    = current->yDir;

        success = true;
    } else if (right) {
        move_idx = move_idx == 0 ? (lut_len-1) : (move_idx-1);
        current = &LUT[move_idx];

        xDir    = current->xDir;
        yDir    = current->yDir;

        success = true;
    }

    return success;
}

void setup_rays() {
    current = &LUT[0];
    xDir = current->xDir;
    yDir = current->yDir;
    xPos = 1152 + VAL_1;
    yPos = 1152 + VAL_1;
    moveSpeed = 128;
    move_idx = 0;
}