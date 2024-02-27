#pragma once

#include <chrono>
#include <functional>

#include <BaseTsd.h>
#include <windows.h>

class GenericTimer
{
public:
   GenericTimer( std::function<void()> timerElapsedFunc,
                 std::chrono::milliseconds timerInterval );
   ~GenericTimer();

   void SetTimerInterval( std::chrono::milliseconds timerInterval );
   void StartTimer();
   void StopTimer();

private:
   static LRESULT CALLBACK WindowProc( HWND   hwnd,
                                       UINT   uMsg,
                                       WPARAM wParam,
                                       LPARAM lParam );

   LRESULT WndProc( UINT   uMsg,
                    WPARAM wParam,
                    LPARAM lParam );

   HWND _hwnd;
   UINT_PTR _timerId = 0;

   std::function<void()> _timerElapsedFunc;
   std::chrono::milliseconds _timerInterval;
};
