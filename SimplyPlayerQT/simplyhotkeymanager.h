#ifndef SIMPLYHOTKEYMANAGER_H
#define SIMPLYHOTKEYMANAGER_H
#ifdef WIN32
#include <Windows.h>
#endif
#include "simplyplayer.h"
#include "simplyfunctions.h"

class SimplyHotkeyManager : public QObject
{
public:
    SimplyHotkeyManager(QWidget *parent = 0);
#ifdef WIN32
    //LRESULT CALLBACK MyLowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);
    void WindowsKeyBoardHook(int code);
#endif

};

#endif // SIMPLYHOTKEYMANAGER_H
