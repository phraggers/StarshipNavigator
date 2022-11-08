#ifndef _STARSHIPNAVIGATOR_H_
#define _STARSHIPNAVIGATOR_H_

#if !(defined _MSC_VER)
#define _MSC_VER 0
#endif
#if !(defined __INTEL_COMPILER)
#define __INTEL_COMPILER 0
#endif
#if _MSC_VER && !__INTEL_COMPILER
#define MSVC 1
#endif

//-=-=-=-=-=-=-=-=-=-=//
//       TYPES        //
//-=-=-=-=-=-=-=-=-=-=//

#define I8_MIN        (-127i8 - 1)
#define I16_MIN       (-32767i16 - 1)
#define I32_MIN       (-2147483647i32 - 1)
#define I64_MIN       (-9223372036854775807i64 - 1)

#define I8_MAX        127i8
#define I16_MAX       32767i16
#define I32_MAX       2147483647i32
#define I64_MAX       9223372036854775807i64

#define U8_MIN        0
#define U16_MIN       0
#define U32_MIN       0
#define U64_MIN       0

#define U8_MAX        0xffui8
#define U16_MAX       0xffffui16
#define U32_MAX       0xffffffffui32
#define U64_MAX       0xffffffffffffffffui64

#define R32_MIN       1.17549e-038f
#define R64_MIN       2.22507e-308

#define R32_MAX       3.40282e+038f
#define R64_MAX       1.79769e+308

#define PI32          3.14159265358f
#define PI64          3.14159265358979323846
#define TAU32         6.28318530718f
#define TAU64         6.28318530717958647692

typedef   char        signed     i8;
typedef   short       signed     i16;
typedef   int         signed     i32;
typedef   long long   signed     i64;

typedef   char        unsigned   u8;
typedef   short       unsigned   u16;
typedef   int         unsigned   u32;
typedef   long long   unsigned   u64;

typedef   float                  r32;
typedef   double                 r64;

#ifndef __cplusplus
typedef   enum { false = 0, true = 1 }   bool;
#endif

#define KB(Value) ((Value)*1024LL)
#define MB(Value) (KB(Value)*1024LL)
#define GB(Value) (MB(Value)*1024LL)
#define TB(Value) (GB(Value)*1024LL)

//-=-=-=-=-=-=-=-=-=-=//
//   DEFAULT PREFS    //
//-=-=-=-=-=-=-=-=-=-=//
// system
#define DEF_APPORG "Phragware"
#define DEF_APPNAME "StarshipNavigator"
#define DEF_APPVER_MAJ 0
#define DEF_APPVER_MIN 1
#define DEF_APPVER_PAT 0
#define DEF_MULTITHREAD 1

// audio
#define DEF_AUDIO_FREQ 48000
#define DEF_AUDIO_FORMAT MIX_DEFAULT_FORMAT
#define DEF_AUDIO_CHANNELS 2
#define DEF_AUDIO_CHUNKSIZE 1024

// window
#define DEF_WINDOW_WIDTH 960
#define DEF_WINDOW_HEIGHT 540

// state
#define DEF_ALLOCSIZE MB(256)

// logging
#define DEF_LOG_BUFFER_SIZE 256
#define DEF_LOG_QUEUE_COUNT 24

// input
#define DEF_INPUT_KEYCOUNT 16
#define DEF_INPUT_DEADZONE 3200
#define DEF_INPUT_CONTROLLERS 4

// timer
#define DEF_TIMER_SLOTS 32

u64 SN_TopOfTheStack;
u64 SN_GetCurrentStackSize(void);

//-=-=-=-=-=-=-=-=-=-=//
//      INCLUDES      //
//-=-=-=-=-=-=-=-=-=-=//
#define _CRT_SECURE_NO_WARNINGS 1
#include <curl/curl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>
#include "StringUtils.h"
#include "Timer.h"
#include "Window.h"
//#include "Audio.h"
#include "Input.h"
#include "Random.h"
#include "Log.h"
#include "State.h"

#ifdef SN_MAIN
#define STB_IMAGE_IMPLEMENTATION
SN_AppState* State;
#else
extern SN_AppState* State;
#endif
#define STBI_MALLOC(sz)           SDL_malloc(sz)
#define STBI_REALLOC(p,newsz)     SDL_realloc(p,newsz)
#define STBI_FREE(p)              SDL_free(p)
#include <stb_image.h>

#endif //_STARSHIPNAVIGATOR_H_