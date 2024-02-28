#pragma once

#include <functional>
#include <vector>

#include <windows.h>

class Dispatcher
{
public:
   Dispatcher();

   void CallOnMainThread( std::function<void()> func );

private:
   static LRESULT CALLBACK WindowProc( HWND   hwnd,
                                       UINT   uMsg,
                                       WPARAM wParam,
                                       LPARAM lParam );

   LRESULT WndProc( UINT   uMsg,
                    WPARAM wParam,
                    LPARAM lParam );

   HWND _hwnd;

private:
   std::vector<std::function<void()>> _functionsToCallOnMainThread;
};
