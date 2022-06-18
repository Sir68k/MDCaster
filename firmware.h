#ifndef FIRMWARE_H
#define FIRMWARE_H

#include <stdint.h>

typedef void func(void);
typedef void func1(uint32_t a);
typedef void func2(uint32_t a, uint32_t b);
typedef int func2i(int a, int b);
typedef uint32_t func2u(uint32_t a, uint32_t b);

// general
static func* start_service_mode = (func*)0x51fe5;


//math 
static func2i* signed_div = (func2i*)0x7cba9;
static func2u* usigned_div = (func2u*)0x07cc81;

// tron

static func1* tron_ter_task = (func1*)0x0007c5c1;
static func1* tron_dly_tsk = (func1*)0x7c201;

// time 
uint32_t get_sys_ltime();

// display
typedef void func_queue_direct_display_cmd(uint8_t* buff, uint32_t len);
static func_queue_direct_display_cmd* queue_direct_display_cmd = (func_queue_direct_display_cmd*)0x0003ecbd;

// button
uint8_t get_key_state();

enum KeyState {
    KeyPrev   = 0x6c,
    KeyNext   = 0x6e,
    KeyStop   = 0x7a,
    KeyPlay   = 0x74,
    KeyPause  = 0x72,
    KeyVolP   = 0x50,
    KeyVolM   = 0x52,
    KeyRec    = 0x78,
    KeyMenu   = 0x20,
    KeyEndS   = 0x32,
    KeyGroup  = 0x37,
    KeyGroupP = 0x66,
    KeyGroupM = 0x64,

    KeyDevice = 0x1
};

// eabi

//typedef uint32_t func__aeabi_uidiv(uint32_t a, uint32_t b);
//static func__aeabi_uidiv*const __aeabi_uidiv = (func__aeabi_uidiv*)0x0007cc801;

//typedef int func__aeabi_idiv(int a, int b);
//static func__aeabi_idiv*const __aeabi_idiv = (func__aeabi_idiv*)0x0007cba9;

#endif