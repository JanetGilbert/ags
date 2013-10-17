/*

Utility functions for Wadjet eye games

*/
//#include <string>

//using namespace std;


#ifdef WIN32
#define WINDOWS_VERSION
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma warning(disable : 4244)
#endif

#if !defined(BUILTIN_PLUGINS)
#define THIS_IS_THE_PLUGIN
#endif

#include "plugin/agsplugin.h"

#if defined(BUILTIN_PLUGINS)
namespace agswadjetutil {
#endif

IAGSEngine* engine;


#if defined(IOS_VERSION)

extern "C"
{
  void fakekey(int keypress);
  bool isPhone();

  int GetAchievementValue(char * name);
  void SetAchievementValue(char * name, int value);
  void ShowAchievements();
  void ResetAchievements();


}

#endif



// ********************************************
// ************  AGS Interface  ***************
// ********************************************
    
// Phone or pad?
bool IsOnPhone()
{
#if defined(IOS_VERSION)
    return isPhone();
#endif
	return false;
}
    

void FakeKeypress(int keypress)
{
#if defined(IOS_VERSION)
  fakekey(keypress);
#endif
}

void IosSetAchievementValue(char * name, int value)
{
/*#if defined(IOS_VERSION)
  SetAchievementValue(name, value);
#endif*/
}


int IosGetAchievementValue(char * name)
{
/*#if defined(IOS_VERSION)
  return GetAchievementValue(name);
#endif*/
  return -1;
}

void IosShowAchievements()
{
/*#if defined(IOS_VERSION)
  //ShowAchievements(); doesn't work
#endif*/
}

void IosResetAchievements()
{
/*#if defined(IOS_VERSION)
    ResetAchievements();
#endif*/
}

void AGS_EngineStartup(IAGSEngine *lpEngine)
{
  engine = lpEngine;

  engine->RegisterScriptFunction("FakeKeypress", (void*)&FakeKeypress);
  engine->RegisterScriptFunction("IsOnPhone", (void*)&IsOnPhone);
  engine->RegisterScriptFunction("IosGetAchievementValue", (int*)&IosGetAchievementValue);
  engine->RegisterScriptFunction("IosSetAchievementValue", (void*)&IosSetAchievementValue);
  engine->RegisterScriptFunction("IosShowAchievements", (void*)&IosShowAchievements);
  engine->RegisterScriptFunction("IosResetAchievements", (void*)&IosResetAchievements);
}

void AGS_EngineShutdown()
{

}

int AGS_EngineOnEvent(int event, int data)
{
  return 0;
}

int AGS_EngineDebugHook(const char *scriptName, int lineNum, int reserved)
{
  return 0;
}

void AGS_EngineInitGfx(const char *driverID, void *data)
{
}



#if defined(WINDOWS_VERSION) && !defined(BUILTIN_PLUGINS)

// ********************************************
// ***********  Editor Interface  *************
// ********************************************
    
    
const char* scriptHeader =
  "import void FakeKeypress(int);\r\n"
  "import bool IsOnPhone();\r\n"
  "import int IosGetAchievementValue(String);\r\n"
  "import bool IosSetAchievementValue(String, int);\r\n"
  "import void IosShowAchievements();\r\n"
  "import void IosResetAchievements();\r\n"
  ;


IAGSEditor* editor;


LPCSTR AGS_GetPluginName(void)
{
  // Return the plugin description
  return "Wadjet eye utilities";
}

int  AGS_EditorStartup(IAGSEditor* lpEditor)
{
  // User has checked the plugin to use it in their game

  // If it's an earlier version than what we need, abort.
  if (lpEditor->version < 1)
    return -1;

  editor = lpEditor;
  editor->RegisterScriptHeader(scriptHeader);

  // Return 0 to indicate success
  return 0;
}

void AGS_EditorShutdown()
{
  // User has un-checked the plugin from their game
  editor->UnregisterScriptHeader(scriptHeader);
}

void AGS_EditorProperties(HWND parent)
{
  // User has chosen to view the Properties of the plugin
  // We could load up an options dialog or something here instead
  MessageBoxA(parent, "Wadjet eye utilities plugin by Janet Gilbert", "About", MB_OK | MB_ICONINFORMATION);
}

int AGS_EditorSaveGame(char* buffer, int bufsize)
{
  // We don't want to save any persistent data
  return 0;
}

void AGS_EditorLoadGame(char* buffer, int bufsize)
{
  // Nothing to load for this plugin
}

#endif


#if defined(BUILTIN_PLUGINS)
}
#endif
