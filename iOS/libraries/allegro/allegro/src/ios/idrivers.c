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
 *      List of iOS drivers.
 *
 *      By JJS for the Adventure Game Studio runtime port.
 *      Based on the Allegro PSP port.
 *
 *      See readme.txt for copyright information.
 */


#include "allegro.h"

#ifndef ALLEGRO_IOS
#error Something is wrong with the makefile
#endif


_DRIVER_INFO _system_driver_list[] =
{
   { SYSTEM_IOS,              &system_ios,              TRUE  },
   { SYSTEM_NONE,             &system_none,             FALSE },
   { 0,                       NULL,                     0     }
};


_DRIVER_INFO _keyboard_driver_list[] =
{
   { KEYBOARD_IOS,            &keyboard_ios,            TRUE  },
   { 0,                       NULL,                     0     }
};


_DRIVER_INFO _timer_driver_list[] =
{
   { TIMER_IOS,               &timer_ios,               TRUE  },
   { 0,                       NULL,                     0     }
};


_DRIVER_INFO _mouse_driver_list[] =
{
   { MOUSE_IOS,               &mouse_ios,               TRUE  },
   { 0,                       NULL,                     0     }
};


_DRIVER_INFO _gfx_driver_list[] =
{
   { GFX_IOS,                 &gfx_ios,                 TRUE  },
   { 0,                       NULL,                     0     }
};


_DRIVER_INFO _digi_driver_list[] =
{
   { DIGI_IOS,                &digi_ios,                TRUE  },
   { 0,                       NULL,                     0     }
};


BEGIN_MIDI_DRIVER_LIST
MIDI_DRIVER_DIGMID
END_MIDI_DRIVER_LIST

BEGIN_JOYSTICK_DRIVER_LIST
END_JOYSTICK_DRIVER_LIST
