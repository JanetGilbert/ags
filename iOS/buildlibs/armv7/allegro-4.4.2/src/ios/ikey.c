/*         ______   ___    ___
*        /\  _  \ /\_ \  /\_ \
*        \ \ \\ \\//\ \ \//\ \      __     __   _ __   ___
*         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
*          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
*           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
*            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
*                                           /\____/
*                                           \_/__/
*
*      Keyboard driver.
*
*      By JJS for the Adventure Game Studio runtime port.
*      Based on the Allegro PSP port.
*
*      See readme.txt for copyright information.
*/


#include "allegro.h"
#include "allegro/internal/aintern.h"
#include "allegro/platform/aintios.h"

#ifndef ALLEGRO_IOS
#error Something is wrong with the makefile
#endif


static int ios_keyboard_init(void);
static void ios_keyboard_exit(void);
static void ios_keyboard_poll(void);
static int ios_scancode_to_ascii(int scancode);

extern int ios_get_last_keypress();


static int ios_ascii_to_allegro[] = {
   -1, // 0
   -1, // 1
   -1, // 2
   -1, // 3
   -1, // 4
   -1, // 5
   -1, // 6
   -1, // 7
   KEY_BACKSPACE, // 8
   KEY_TAB, // 9
   KEY_ENTER, // 10
   -1, // 11
   -1, // 12
   KEY_ENTER, // 13
   -1, // 14
   -1, // 15
   -1, // 16
   -1, // 17
   -1, // 18
   -1, // 19
   -1, // 20
   -1, // 21
   -1, // 22
   -1, // 23
   -1, // 24
   -1, // 25
   -1, // 26
   KEY_ESC, // 27
   -1, // 28
   -1, // 29
   -1, // 30
   -1, // 31
   KEY_SPACE, // 32
   -1, // 33
   KEY_QUOTE, // 34
   -1, // 35
   -1, // 36
   -1, // 37
   -1, // 38
   -1, // 39
   KEY_OPENBRACE, // 40
   KEY_CLOSEBRACE, // 41
   KEY_ASTERISK, // 42
   KEY_PLUS_PAD, // 43
   KEY_COMMA, // 44
   -1, // 45
   KEY_STOP, // 46
   KEY_SLASH, // 47
   KEY_0, // 48
   KEY_1, // 49
   KEY_2, // 50
   KEY_3, // 51
   KEY_4, // 52
   KEY_5, // 53
   KEY_6, // 54
   KEY_7, // 55
   KEY_8, // 56
   KEY_9, // 57
   KEY_COLON, // 58
   KEY_SEMICOLON, // 59
   -1, // 60
   KEY_EQUALS, // 61
   -1, // 62
   -1, // 63
   KEY_AT, // 64
   KEY_A, // 65
   KEY_B, // 66
   KEY_C, // 67
   KEY_D, // 68
   KEY_E, // 69
   KEY_F, // 70
   KEY_G, // 71
   KEY_H, // 72
   KEY_I, // 73
   KEY_J, // 74
   KEY_K, // 75
   KEY_L, // 76
   KEY_M, // 77
   KEY_N, // 78
   KEY_O, // 79
   KEY_P, // 80
   KEY_Q, // 81
   KEY_R, // 82
   KEY_S, // 83
   KEY_T, // 84
   KEY_U, // 85
   KEY_V, // 86
   KEY_W, // 87
   KEY_X, // 88
   KEY_Y, // 89
   KEY_Z, // 90
   -1, // 91
   KEY_BACKSLASH, // 92
   -1, // 93
   KEY_CIRCUMFLEX, // 94
   -1, // 95
   -1, // 96
   KEY_A, // 97
   KEY_B, // 98
   KEY_C, // 99
   KEY_D, // 100
   KEY_E, // 101
   KEY_F, // 102
   KEY_G, // 103
   KEY_H, // 104
   KEY_I, // 105
   KEY_J, // 106
   KEY_K, // 107
   KEY_L, // 108
   KEY_M, // 109
   KEY_N, // 110
   KEY_O, // 111
   KEY_P, // 112
   KEY_Q, // 113
   KEY_R, // 114
   KEY_S, // 115
   KEY_T, // 116
   KEY_U, // 117
   KEY_V, // 118
   KEY_W, // 119
   KEY_X, // 120
   KEY_Y, // 121
   KEY_Z, // 122
   -1, // 123
   -1, // 124
   -1, // 125
   KEY_TILDE, // 126
   -1 // 127
};


KEYBOARD_DRIVER keyboard_ios =
{
   KEYBOARD_IOS,
   empty_string,
   empty_string,
   "iOS keyboard",
   FALSE,  // int autorepeat;
   ios_keyboard_init,
   ios_keyboard_exit,
   ios_keyboard_poll,
   NULL,   // AL_METHOD(void, set_leds, (int leds));
   NULL,   // AL_METHOD(void, set_rate, (int delay, int rate));
   NULL,   // AL_METHOD(void, wait_for_input, (void));
   NULL,   // AL_METHOD(void, stop_waiting_for_input, (void));
   NULL,   // AL_METHOD(int,  scancode_to_ascii, (int scancode));
   NULL    // scancode_to_name
};


static int ios_last_scancode = -1;


static int ios_ascii_to_allegro_keycode(int* ascii)
{
   if (*ascii > 127)
   {
      int result = *ascii - 0x1000;
     *ascii = 0;
     return result;
   }
   else
      return ios_ascii_to_allegro[*ascii];
}


static int ios_keyboard_init(void)
{
   return 0;
}


static void ios_keyboard_exit(void)
{
}


static void ios_keyboard_poll(void)
{
   int ascii_code;
   int allegro_keycode;
   
   // Hold the key down till the next keyboard poll
   if (ios_last_scancode != -1)
   {
      _handle_key_release(ios_last_scancode); 
      ios_last_scancode = -1;
   }   
   
   ascii_code = ios_get_last_keypress();
   if (ascii_code < 1)
      return;
   
   allegro_keycode = ios_ascii_to_allegro_keycode(&ascii_code);
   
   if (allegro_keycode == -1)
      return;

   // Replace LF with CR
   if (ascii_code == 10)
      ascii_code = 13;

   _handle_key_press(ascii_code, allegro_keycode);

   ios_last_scancode = allegro_keycode;
}
