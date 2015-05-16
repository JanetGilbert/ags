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
 *      Mouse driver.
 *
 *      By JJS for the Adventure Game Studio runtime port.
 *      Based on the Allegro PSP port.
 *
 *      See readme.txt for copyright information.
 */

#include "allegro.h"
#include "allegro/internal/aintern.h"
#include "allegro/platform/aintios.h"

#include "math.h"

#ifndef ALLEGRO_IOS
#error something is wrong with the makefile
#endif

static int  ios_mouse_init(void);
static void ios_mouse_exit(void);
static void ios_mouse_position(int, int);
static void ios_mouse_set_range(int, int, int, int);
static void ios_mouse_get_mickeys(int *, int *);
static void ios_mouse_poll(void);

extern void ios_poll_mouse_relative(int* x, int* y);
extern void ios_poll_mouse_absolute(int* x, int* y);
extern int ios_poll_mouse_buttons();

static int mouse_minx = 0;
static int mouse_miny = 0;
static int mouse_maxx = 320;
static int mouse_maxy = 200;
int ios_left_mouse_stay = 0;
int ios_right_mouse_stay = 0;
int ios_mouse_last_x = 0;
int ios_mouse_last_y = 0;
int ios_mouse_clip_left = 0;
int ios_mouse_clip_right = 479;
int ios_mouse_clip_top = 0;
int ios_mouse_clip_bottom = 319;
float ios_mouse_scaling_x = 1.0f;
float ios_mouse_scaling_y = 1.0f;

int config_mouse_control_mode = 0; // 0 = direct, 1 = relative


MOUSE_DRIVER mouse_ios =
{
   MOUSE_IOS,
   empty_string,
   empty_string,
   "iOS mouse",
   ios_mouse_init,
   ios_mouse_exit,
   ios_mouse_poll,       // AL_METHOD(void, poll, (void));
   NULL,                 // AL_METHOD(void, timer_poll, (void));
   ios_mouse_position,
   ios_mouse_set_range,
   NULL,       // AL_METHOD(void, set_speed, (int xspeed, int yspeed));
   ios_mouse_get_mickeys,
   NULL,       // AL_METHOD(int,  analyse_data, (AL_CONST char *buffer, int size));
   NULL,       // AL_METHOD(void,  enable_hardware_cursor, (AL_CONST int mode));
   NULL        // AL_METHOD(int,  select_system_cursor, (AL_CONST int cursor));
};



static int ios_mouse_init(void)
{
   return 3; //Num of buttons.
}


void ios_clip_mouse(int* x, int* y)
{
   if (*x < ios_mouse_clip_left)
      *x = ios_mouse_clip_left;

   if (*y < ios_mouse_clip_top)
      *y = ios_mouse_clip_top;

   if (*x > ios_mouse_clip_right)
      *x = ios_mouse_clip_right;

   if (*y > ios_mouse_clip_bottom)
      *y = ios_mouse_clip_bottom;

   *x -= ios_mouse_clip_left;
   *y -= ios_mouse_clip_top;
}


void ios_scale_mouse(int* x, int* y)
{
   *x = (float)*x * ios_mouse_scaling_x;
   *y = (float)*y * ios_mouse_scaling_y;
}


void ios_mouse_setup(int left, int right, int top, int bottom, float scaling_x, float scaling_y)
{
   ios_mouse_clip_left = left;
   ios_mouse_clip_right = right;
   ios_mouse_clip_top = top;
   ios_mouse_clip_bottom = bottom;
   ios_mouse_scaling_x = scaling_x;
   ios_mouse_scaling_y = scaling_y;
}


static void ios_mouse_poll(void)
{

   int new_x;
   int new_y;
   int new_click = ios_poll_mouse_buttons();

   if (config_mouse_control_mode == 1)
   {
      ios_poll_mouse_relative(&new_x, &new_y);
      ios_scale_mouse(&new_x, &new_y);
   
      new_x += _mouse_x;
      new_y += _mouse_y;
   }
   else
   {
      ios_poll_mouse_absolute(&new_x, &new_y);
      ios_clip_mouse(&new_x, &new_y);	
      ios_scale_mouse(&new_x, &new_y);
   }
   
   if (new_x < mouse_minx)
      new_x = mouse_minx;

   if (new_y < mouse_miny)
      new_y = mouse_miny;

   if (new_x > mouse_maxx)
      new_x = mouse_maxx;

   if (new_y > mouse_maxy)
      new_y = mouse_maxy;

   _mouse_x = new_x;
   _mouse_y = new_y;
   
   
   if (new_click == 1)
      ios_left_mouse_stay = 10;
   else if (new_click == 2)
      ios_right_mouse_stay = 10;
   else if (new_click == 10)
      ios_left_mouse_stay = 10 * 1000 * 1000;
   
   if (ios_left_mouse_stay > 0)
      ios_left_mouse_stay--;

   if (ios_right_mouse_stay > 0)
      ios_right_mouse_stay--;

   if (ios_left_mouse_stay > 0)
      _mouse_b = 1;
   else if (ios_right_mouse_stay > 0)
      _mouse_b = 2;
   else
      _mouse_b = 0;
}




static void ios_mouse_position(int x, int y)
{
   _mouse_x = x;
   _mouse_y = y;
}



static void ios_mouse_set_range(int x1, int y1, int x2, int y2)
{
   mouse_minx = x1;
   mouse_miny = y1;
   mouse_maxx = x2;
   mouse_maxy = y2;
}



static void ios_mouse_get_mickeys(int *mickeyx, int *mickeyy)
{

}



static void ios_mouse_exit(void)
{
}

