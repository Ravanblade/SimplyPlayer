#include "simplyhotkeymanager.h"
#include "simplyfunctions.h"

SimplyHotkeyManager *SimplyHotkeyManager;
LRESULT CALLBACK MyLowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);
HHOOK hHook = NULL;

SimplyHotkeyManager::SimplyHotkeyManager(QWidget *parent)
{

#ifdef WIN32
    //   hHook = NULL;
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, MyLowLevelKeyBoardProc, NULL, 0);
    if(hHook == NULL)
        qDebug() << "Hook failed";

#endif


}


#ifdef WIN32

LRESULT CALLBACK MyLowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    //WPARAM is WM_KEYDOWN, WM_KEYUP, WM_SYSKEYDOWN, or WM_SYSKEYUP
    //LPARAM is the key information

    //Get the key information
    KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT*)lParam);

    if(wParam == WM_KEYDOWN)
    {

        if(cKey.vkCode == 179)
            SimplyFunctions::SendSignal("PlayPause",0,0);
        else if(cKey.vkCode == 176)
            SimplyFunctions::SendSignal("Forward",0,0);
        else if(cKey.vkCode == 177)
            SimplyFunctions::SendSignal("Backward",0,0);
        else if(cKey.vkCode == 178)
            SimplyFunctions::SendSignal("Stop",0,0);


    }



    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

#endif
