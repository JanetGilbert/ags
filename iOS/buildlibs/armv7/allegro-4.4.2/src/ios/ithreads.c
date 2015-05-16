/*         ______   ___    ___
 *        /\  _  \ /\_ \  /\_ \
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      Asynchronous event processing with pthreads.
 *
 *      By George Foot and Peter Wang.
 *
 *      Synchronization functions added by Eric Botcazou.
 *
 *      Copied in part for the AGS iOS port.
 *
 *      See readme.txt for copyright information.
 */


#include "allegro.h"
#include "allegro/internal/aintern.h"
#include "allegro/platform/aintios.h"

#ifndef ALLEGRO_IOS
   #error something is wrong with the makefile
#endif

#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <limits.h>


/* custom mutex that supports nested locking */
struct my_mutex {
   int lock_count;                /* level of nested locking     */
   pthread_t owner;               /* thread which owns the mutex */
   pthread_mutex_t actual_mutex;  /* underlying mutex object     */
};



/* _ios_create_mutex:
 *  Creates a mutex and returns a pointer to it.
 */
void *_ios_create_mutex(void)
{
   struct my_mutex *mx;

   mx = _AL_MALLOC(sizeof(struct my_mutex));
   if (!mx) {
      *allegro_errno = ENOMEM;
      return NULL;
   }

   mx->lock_count = 0;
   mx->owner = (pthread_t) 0;

   pthread_mutex_init(&mx->actual_mutex, NULL);

   return (void *)mx;
}



/* _ios_destroy_mutex:
 *  Destroys a mutex.
 */
void _ios_destroy_mutex(void *handle)
{
   struct my_mutex *mx = (struct my_mutex *)handle;

   pthread_mutex_destroy(&mx->actual_mutex);

   _AL_FREE(mx);
}



/* _ios_lock_mutex:
 *  Locks a mutex.
 */
void _ios_lock_mutex(void *handle)
{
   struct my_mutex *mx = (struct my_mutex *)handle;

   if (mx->owner != pthread_self()) {
      pthread_mutex_lock(&mx->actual_mutex);
      mx->owner = pthread_self();      
   }

   mx->lock_count++;
}



/* _ios_unlock_mutex:
 *  Unlocks a mutex.
 */
void _ios_unlock_mutex(void *handle)
{
   struct my_mutex *mx = (struct my_mutex *)handle;

   mx->lock_count--;

   if (mx->lock_count == 0) {
      mx->owner = (pthread_t) 0;
      pthread_mutex_unlock(&mx->actual_mutex);
   }
}
