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
*      Display driver.
*
*      By JJS for the Adventure Game Studio runtime port.
*      Based on the Allegro PSP port.
*
*      See readme.txt for copyright information.
*/

#include <../../System/Library/Frameworks/OpenGLES.framework/Headers/ES1/gl.h>

#include "allegro.h"
#include "allegro/internal/aintern.h"
#include "allegro/platform/aintios.h"


extern void ios_swap_buffers();
extern void ios_select_buffer();


/* Software version of some blitting methods */
static void (*_orig_blit) (BITMAP * source, BITMAP * dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);

static BITMAP *ios_display_init(int w, int h, int v_w, int v_h, int color_depth);
static void gfx_ios_enable_acceleration(GFX_VTABLE *vtable);
static void ios_hw_blit(BITMAP *source, BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);


void ios_render();
void ios_initialize_opengl();


/* Options controlled by the application */
int psp_gfx_scaling = 1;
int psp_gfx_smoothing = 1;


BITMAP* displayed_video_bitmap;

unsigned int ios_screen_texture = 0;
unsigned int ios_screen_width = 320;
unsigned int ios_screen_height = 200;
unsigned int ios_screen_physical_width = 320;
unsigned int ios_screen_physical_height = 200;
unsigned int ios_screen_color_depth = 32;
int ios_screen_texture_width = 0;
int ios_screen_texture_height = 0;
float ios_screen_ar = 1.0f;
float ios_device_ar = 1.0f;

volatile int ios_screen_is_dirty = 0;
int ios_screen_initialized = 0;


GLfloat ios_vertices[] =
{
   0, 0,
   320,  0,
   0,  200,
   320,  200
};

GLfloat ios_texture_coordinates[] =
{
   0, 200.0f / 256.0f,
   320.0f / 512.0f, 200.0f / 256.0f,
   0, 0,
   320.0f / 512.0f, 0
};


GFX_DRIVER gfx_ios =
{
   GFX_IOS,
   empty_string,
   empty_string,
   "iOS gfx driver",
   ios_display_init,             /* AL_METHOD(struct BITMAP *, init, (int w, int h, int v_w, int v_h, int color_depth)); */
   NULL,                         /* AL_METHOD(void, exit, (struct BITMAP *b)); */
   NULL,                         /* AL_METHOD(int, scroll, (int x, int y)); */
   NULL,                         /* AL_METHOD(void, vsync, (void)); */
   NULL,                         /* AL_METHOD(void, set_palette, (AL_CONST struct RGB *p, int from, int to, int retracesync)); */
   NULL,                         /* AL_METHOD(int, request_scroll, (int x, int y)); */
   NULL,                         /* AL_METHOD(int, poll_scroll, (void)); */
   NULL,                         /* AL_METHOD(void, enable_triple_buffer, (void)); */
   NULL,                         /* AL_METHOD(struct BITMAP *, create_video_bitmap, (int width, int height)); */
   NULL,                         /* AL_METHOD(void, destroy_video_bitmap, (struct BITMAP *bitmap)); */
   NULL,                         /* AL_METHOD(int, show_video_bitmap, (BITMAP *bitmap)); */
   NULL,                         /* AL_METHOD(int, request_video_bitmap, (BITMAP *bitmap)); */
   NULL,                         /* AL_METHOD(BITMAP *, create_system_bitmap, (int width, int height)); */
   NULL,                         /* AL_METHOD(void, destroy_system_bitmap, (BITMAP *bitmap)); */
   NULL,                         /* AL_METHOD(int, set_mouse_sprite, (BITMAP *sprite, int xfocus, int yfocus)); */
   NULL,                         /* AL_METHOD(int, show_mouse, (BITMAP *bmp, int x, int y)); */
   NULL,                         /* AL_METHOD(void, hide_mouse, (void)); */
   NULL,                         /* AL_METHOD(void, move_mouse, (int x, int y)); */
   NULL,                         /* AL_METHOD(void, drawing_mode, (void)); */
   NULL,                         /* AL_METHOD(void, save_video_state, (void)); */
   NULL,                         /* AL_METHOD(void, restore_video_state, (void)); */
   NULL,                         /* AL_METHOD(void, set_blender_mode, (int mode, int r, int g, int b, int a)); */
   NULL,                         /* AL_METHOD(int, fetch_mode_list, (void)); */
   0, 0,                         /* physical (not virtual!) screen size */
   TRUE,                         /* true if video memory is linear */
   0,                            /* bank size, in bytes */
   0,                            /* bank granularity, in bytes */
   0,                            /* video memory size, in bytes */
   0,                            /* physical address of video memory */
   FALSE                         /* true if driver runs windowed */
};


extern void ios_create_screen();

static BITMAP *ios_display_init(int w, int h, int v_w, int v_h, int color_depth)
{
   GFX_VTABLE* vtable = _get_vtable(color_depth);

   /* Do the final screen blit to a 32 bit bitmap in palette mode */
   if (color_depth == 8)
      color_depth = 32;

   ios_screen_width = w;
   ios_screen_height = h;
   ios_screen_color_depth = color_depth;

   displayed_video_bitmap = create_bitmap_ex(color_depth, w, h);
   gfx_ios_enable_acceleration(vtable);

   ios_create_screen();
   
   return displayed_video_bitmap;
}


static void gfx_ios_enable_acceleration(GFX_VTABLE *vtable)
{
   /* Keep the original blitting methods */
   _orig_blit = vtable->blit_to_self;

   /* Accelerated blits. */
   vtable->blit_from_memory = ios_hw_blit;
   vtable->blit_to_memory = ios_hw_blit;
   vtable->blit_from_system = ios_hw_blit;
   vtable->blit_to_system = ios_hw_blit;
   vtable->blit_to_self = ios_hw_blit;

   _screen_vtable.blit_from_memory = ios_hw_blit;
   _screen_vtable.blit_to_memory = ios_hw_blit;
   _screen_vtable.blit_from_system = ios_hw_blit;
   _screen_vtable.blit_to_system = ios_hw_blit;
   _screen_vtable.blit_to_self = ios_hw_blit;

   gfx_capabilities |= (GFX_HW_VRAM_BLIT | GFX_HW_MEM_BLIT);
}


/* All blitting goes through here, signal a screen update if the blit target is the screen bitmap */
static void ios_hw_blit(BITMAP *source, BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height)
{
   _orig_blit(source, dest, source_x, source_y, dest_x, dest_y, width, height);

   if (dest == displayed_video_bitmap)
      ios_render();
}


int ios_get_next_power_of_2(int value)
{
   int test = 1;

   while (test < value)
      test *= 2;

   return test;
}


/* Create the texture that holds the screen bitmap */
void ios_create_screen_texture(int width, int height, int color_depth)
{
   char* empty;

   ios_screen_texture_width = ios_get_next_power_of_2(width);
   ios_screen_texture_height = ios_get_next_power_of_2(height);

   empty = (char*)malloc(ios_screen_texture_width * ios_screen_texture_height * color_depth / 8);
   memset(empty, 0, ios_screen_texture_width * ios_screen_texture_height * color_depth / 8);

   if (ios_screen_texture != 0)
      glDeleteTextures(1, &ios_screen_texture);

   glGenTextures(1, &ios_screen_texture);
   glBindTexture(GL_TEXTURE_2D, ios_screen_texture);

   if (color_depth == 16)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ios_screen_texture_width, ios_screen_texture_height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, empty);
   else
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ios_screen_texture_width, ios_screen_texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, empty);

   free(empty);
}


/* Set the values for the texture coord. and vertex arrays */
void ios_create_arrays()
{
   if (psp_gfx_scaling == 1)
   {
      if (ios_device_ar <= ios_screen_ar)
      {
         ios_vertices[2] = ios_vertices[6] = ios_screen_physical_width - 1;
         ios_vertices[5] = ios_vertices[7] = ios_screen_physical_width * ((float)ios_screen_height / (float)ios_screen_width);
         
        ios_mouse_setup(
            0, 
            ios_screen_physical_width - 1, 
            (ios_screen_physical_height - ios_vertices[5]) / 2, 
            ios_screen_physical_height - ((ios_screen_physical_height - ios_vertices[5]) / 2), 
            (float)ios_screen_width / (float)ios_screen_physical_width, 
            (float)ios_screen_height / ios_vertices[5]);
      }
      else
      {
         ios_vertices[2] = ios_vertices[6] = ios_screen_physical_height * ((float)ios_screen_width / (float)ios_screen_height);
         ios_vertices[5] = ios_vertices[7] = ios_screen_physical_height - 1;
         
         ios_mouse_setup(
            (ios_screen_physical_width - ios_vertices[2]) / 2,
            ios_screen_physical_width - ((ios_screen_physical_width - ios_vertices[2]) / 2),
            0,
            ios_screen_physical_height - 1,
            (float)ios_screen_width / ios_vertices[2], 
            (float)ios_screen_height / (float)ios_screen_physical_height);
      }
   }
   else if (psp_gfx_scaling == 2)
   {
      ios_vertices[2] = ios_vertices[6] = ios_screen_physical_width - 1;
      ios_vertices[5] = ios_vertices[7] = ios_screen_physical_height - 1;
      
      ios_mouse_setup(
         0, 
         ios_screen_physical_width - 1, 
         0, 
         ios_screen_physical_width - 1, 
         (float)ios_screen_width / (float)ios_screen_physical_width, 
         (float)ios_screen_height / (float)ios_screen_physical_height);     
   }   
   else
   {
      ios_vertices[0] = ios_vertices[4] = ios_screen_width * (-0.5f);
      ios_vertices[2] = ios_vertices[6] = ios_screen_width * 0.5f;
      ios_vertices[5] = ios_vertices[7] = ios_screen_height * 0.5f;
      ios_vertices[1] = ios_vertices[3] = ios_screen_height * (-0.5f);
      
      ios_mouse_setup(
         (ios_screen_physical_width - ios_screen_width) / 2,
         ios_screen_physical_width - ((ios_screen_physical_width - ios_screen_width) / 2),
         (ios_screen_physical_height - ios_screen_height) / 2, 
         ios_screen_physical_height - ((ios_screen_physical_height - ios_screen_height) / 2), 
         1.0f,
         1.0f);
   }

   ios_texture_coordinates[1] = ios_texture_coordinates[3] = (float)ios_screen_height / (float)ios_screen_texture_height;
   ios_texture_coordinates[2] = ios_texture_coordinates[6] = (float)ios_screen_width / (float)ios_screen_texture_width;
}


/* Called from the Java app to set up the screen */
void ios_initialize_renderer(int screen_width, int screen_height)
{
   ios_screen_physical_width = screen_width;
   ios_screen_physical_height = screen_height;
   ios_screen_initialized = 0;
}


void ios_initialize_opengl()
{
   ios_screen_ar = (float)ios_screen_width / (float)ios_screen_height;
   ios_device_ar = (float)ios_screen_physical_width / (float)ios_screen_physical_height;

   glEnable(GL_CULL_FACE);
   glEnable(GL_TEXTURE_2D);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_LIGHTING);
   glDisable(GL_BLEND);
   glDisable(GL_SCISSOR_TEST);
   glShadeModel(GL_FLAT);

   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

   glViewport(0, 0, ios_screen_physical_width, ios_screen_physical_height);
   glScissor(0, 0, ios_screen_physical_width, ios_screen_physical_height);
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);

   glDisableClientState(GL_COLOR_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   glTexCoordPointer(2, GL_FLOAT, 0, ios_texture_coordinates);
   glVertexPointer(2, GL_FLOAT, 0, ios_vertices);

   ios_create_screen_texture(ios_screen_width, ios_screen_height, ios_screen_color_depth);
   ios_create_arrays();

   glBindTexture(GL_TEXTURE_2D, ios_screen_texture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

   if (psp_gfx_smoothing)
   {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   }
   else
   {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   }

   glViewport(0, 0, ios_screen_physical_width, ios_screen_physical_height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   glOrthof(0, ios_screen_physical_width - 1, 0, ios_screen_physical_height - 1, 0, 1);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   if (psp_gfx_scaling == 1)
   {
      if (ios_device_ar <= ios_screen_ar)
         glTranslatef(0, (ios_screen_physical_height - ios_vertices[5] - 1) / 2, 0);
      else
         glTranslatef((ios_screen_physical_width - ios_vertices[2] - 1) / 2, 0, 0);
   }
   else if (psp_gfx_scaling == 0)
   {
      glTranslatef(ios_screen_physical_width / 2.0f, ios_screen_physical_height / 2.0f, 0);
   }
}


void ios_render()
{
   ios_select_buffer();

   if (!ios_screen_initialized)
   {
      ios_initialize_opengl();
      ios_screen_initialized = 1;
   }

   if (ios_screen_color_depth == 16)
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ios_screen_width, ios_screen_height, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, displayed_video_bitmap->line[0]);
   else
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ios_screen_width, ios_screen_height, GL_RGBA, GL_UNSIGNED_BYTE, displayed_video_bitmap->line[0]);

   glClear(GL_COLOR_BUFFER_BIT);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

   glFinish();

   ios_swap_buffers();	  
}
