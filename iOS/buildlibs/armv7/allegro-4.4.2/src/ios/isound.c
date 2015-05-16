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
 *      Audio driver.
 *
 *      By JJS for the Adventure Game Studio runtime port.
 *      Based on the Allegro PSP port.
 *
 *      See readme.txt for copyright information.
 */

#include <AudioToolbox/AudioToolbox.h>
#include <pthread.h>
#include "allegro.h"
#include "allegro/internal/aintern.h"
#include "allegro/platform/aintios.h"

#ifndef ALLEGRO_IOS
#error something is wrong with the makefile
#endif


#define SAMPLES_PER_BUFFER 1024


static int digi_ios_detect(int);
static int digi_ios_init(int, int);
static void digi_ios_exit(int);
static void ios_start_audio_queue();

static short sound_buffer[2][SAMPLES_PER_BUFFER][2];
int curr_buffer = 0;

AudioQueueRef ios_queue;
AudioQueueBufferRef ios_queue_buffer[3];
AudioStreamBasicDescription ios_data_format;

volatile int ios_audio_playing = 0;
volatile int ios_audio_interrupted = 0;
volatile int ios_audio_must_restart = 0;

DIGI_DRIVER digi_ios =
{
   DIGI_IOS,
   empty_string,
   empty_string,
   "iOS digital sound driver",
   0,
   0,
   MIXER_MAX_SFX,
   MIXER_DEF_SFX,

   digi_ios_detect,
   digi_ios_init,
   digi_ios_exit,
   NULL,
   NULL,

   NULL,
   NULL,
   NULL,
   _mixer_init_voice,
   _mixer_release_voice,
   _mixer_start_voice,
   _mixer_stop_voice,
   _mixer_loop_voice,

   _mixer_get_position,
   _mixer_set_position,

   _mixer_get_volume,
   _mixer_set_volume,
   _mixer_ramp_volume,
   _mixer_stop_volume_ramp,

   _mixer_get_frequency,
   _mixer_set_frequency,
   _mixer_sweep_frequency,
   _mixer_stop_frequency_sweep,

   _mixer_get_pan,
   _mixer_set_pan,
   _mixer_sweep_pan,
   _mixer_stop_pan_sweep,

   _mixer_set_echo,
   _mixer_set_tremolo,
   _mixer_set_vibrato,
   0, 0,
   0,
   0,
   0,
   0,
   0,
   0
};



static int digi_ios_detect(int input)
{
   return TRUE;
}


void ios_resume_sound()
{
   if (ios_audio_must_restart)
   {
      ios_audio_must_restart = 0;
      AudioQueueStart(ios_queue, NULL);
   }
}


void digi_ios_interruption_handler(void *inUserData, UInt32 interruptionState) 
{
   if (interruptionState == kAudioSessionBeginInterruption) 
   {
      ios_audio_interrupted = 1;
      AudioQueueStop(ios_queue, 1);
   }
   else if (interruptionState == kAudioSessionEndInterruption)
   {
      ios_audio_interrupted = 0;
      ios_start_audio_queue();
   }
}


void digi_ios_audio_callback(void* inUserData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer)
{
   void* bufptr = &sound_buffer[curr_buffer];
   
   if (!ios_audio_playing || ios_audio_interrupted)
      return;

   /* Asks to the Allegro mixer to fill the buffer */
   _mix_some_samples((uintptr_t)bufptr, 0, TRUE);

   /* Send mixed buffer to sound card */
   memcpy(inBuffer->mAudioData, bufptr, SAMPLES_PER_BUFFER * 2 * 2);
   inBuffer->mAudioDataByteSize = SAMPLES_PER_BUFFER * 2 * 2;   
   AudioQueueEnqueueBuffer(ios_queue, inBuffer, 0, NULL);

   curr_buffer = !curr_buffer;
}


extern volatile int is_in_foreground;

static void ios_start_audio_queue()
{
   int i;
   
   AudioQueueNewOutput(&ios_data_format, digi_ios_audio_callback, NULL, NULL, NULL, 0, &ios_queue);

   for (i = 0; i < 3; i++)
   {
      AudioQueueAllocateBuffer(ios_queue, SAMPLES_PER_BUFFER * 2 * 2, &ios_queue_buffer[i]);
      memset(ios_queue_buffer[i]->mAudioData, 0, SAMPLES_PER_BUFFER * 2 * 2);
      ios_queue_buffer[i]->mAudioDataByteSize = SAMPLES_PER_BUFFER * 2 * 2;
      AudioQueueEnqueueBuffer(ios_queue, ios_queue_buffer[i], 0, NULL);
   }

   ios_audio_playing = 1;

   /* Error -12985 happens if the app is currently in the background.
      The most likely cause for this is that an interrupting event
      (like a phone call or an alarm) occured and the user dismissed it
      by pressing the power button. This calls the audio resume callback
      but the app is already in the background. Set a flag so that audio
      gets restarted after the app has come to the foreground. */
   if (AudioQueueStart(ios_queue, NULL) == -12985)
   {
      ios_audio_must_restart = 1;
   }
}


static int digi_ios_init(int input, int voices)
{
   _sound_bits = 16;
   _sound_stereo = TRUE;
   _sound_freq = 44100;  
  
   digi_ios.voices = voices;

   ios_data_format.mSampleRate = _sound_freq;    
   ios_data_format.mFormatID = kAudioFormatLinearPCM;
   ios_data_format.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;
   ios_data_format.mBytesPerPacket = 4;
   ios_data_format.mFramesPerPacket = 1;
   ios_data_format.mBytesPerFrame = 4;
   ios_data_format.mChannelsPerFrame = (_sound_stereo ? 2 : 1);
   ios_data_format.mBitsPerChannel = _sound_bits;
   ios_data_format.mReserved = 0;

   if (_mixer_init(SAMPLES_PER_BUFFER * 2, _sound_freq, _sound_stereo, (_sound_bits == 16), &digi_ios.voices))
      return -1;

#ifndef __i386__
   AudioSessionInitialize(NULL, NULL, digi_ios_interruption_handler, NULL);
#endif

   ios_start_audio_queue();

   return 0;
}


static void digi_ios_exit(int input)
{
   ios_audio_playing = 0;
   AudioQueueStop(ios_queue, 0);
}
