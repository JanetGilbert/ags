//
//  plugin_registration.cpp
//  Shivah
//
//  Created by Nick Sonneveld on 26/06/2016.
//
//

#include "plugin_registration.hpp"

#include "agsplugin.h"
#include "agswadjetutil.h"
#include "AGSteam.h"

typedef void (*t_engine_pre_init_callback)(void);
extern void engine_set_pre_init_callback(t_engine_pre_init_callback callback);

static void pl_register_builtin_plugin() {
  
  pl_register_builtin_plugin((InbuiltPluginDetails){
    .filename = "agswadjetutil",
    .engineStartup = agswadjetutil::AGS_EngineStartup,
    .engineShutdown = agswadjetutil::AGS_EngineShutdown,
    .onEvent = agswadjetutil::AGS_EngineOnEvent,
    .initGfxHook = agswadjetutil::AGS_EngineInitGfx,
    .debugHook = agswadjetutil::AGS_EngineDebugHook,
  });
  
  pl_register_builtin_plugin((InbuiltPluginDetails){
    .filename = "agsteam",
    .engineStartup = ags_steam::AGS_EngineStartup,
    .engineShutdown = ags_steam::AGS_EngineShutdown,
    .onEvent = ags_steam::AGS_EngineOnEvent,
  });
  
}


__attribute__((constructor))
static void oninit_register_builtin_plugins() {
  engine_set_pre_init_callback(&pl_register_builtin_plugin);
}
