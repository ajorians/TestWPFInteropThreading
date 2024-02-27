#include "GenericTimer.h"

#include <exception>

#define NEVER_TRANSLATE(x) L ## x

GenericTimer::GenericTimer( std::function<void()> timerElapsedFunc, std::chrono::milliseconds timerInterval )
   : _timerElapsedFunc( timerElapsedFunc )
   , _timerInterval( timerInterval )
{
   //Message-only window: https://docs.microsoft.com/en-us/windows/win32/winmsg/window-features?redirectedfrom=MSDN#message-only-windows
   const wchar_t* className = NEVER_TRANSLATE( "GenericTimer" );
   WNDCLASSEX wx
   {
      .cbSize = sizeof( WNDCLASSEX ),
      .lpfnWndProc = GenericTimer::WindowProc,
      .hInstance = GetModuleHandle( NULL ),
      .lpszClassName = className
   };
   (void)RegisterClassEx( &wx );

   _hwnd = CreateWindowEx( 0, className, NEVER_TRANSLATE( "GenericTimer Message-Only Wnd :)" ), 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL );
   SetWindowLongPtr( _hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( this ) );
}

GenericTimer::~GenericTimer()
{
   StopTimer();
   DestroyWindow( _hwnd );
}

void GenericTimer::SetTimerInterval( std::chrono::milliseconds timerInterval )
{
   _timerInterval = timerInterval;

   //It is caller's responsibility to restart timer if was one currently running
}

void GenericTimer::StartTimer()
{
   StopTimer();

   _timerId = ::SetTimer( _hwnd, 1'000/*id*/, static_cast<UINT>( _timerInterval.count() ), NULL );
}

void GenericTimer::StopTimer()
{
   if ( _timerId != 0 )
   {
      if ( FALSE != ::KillTimer( _hwnd, _timerId ) )
      {
         _timerId = 0;
      }
   }
}

/*static*/
LRESULT CALLBACK GenericTimer::WindowProc( HWND   hwnd,
                                           UINT   uMsg,
                                           WPARAM wParam,
                                           LPARAM lParam )
{
   if ( GenericTimer* pTimer = reinterpret_cast<GenericTimer*>( GetWindowLongPtr( hwnd, GWLP_USERDATA ) );
        pTimer != nullptr )
   {
      return pTimer->WndProc( uMsg, wParam, lParam );
   }

   return ::DefWindowProc( hwnd, uMsg, wParam, lParam );
}

LRESULT GenericTimer::WndProc( UINT   uMsg,
                               WPARAM wParam,
                               LPARAM lParam )
{
   if ( uMsg == WM_TIMER )
   {
      _timerElapsedFunc();
      return 1L;
   }

   return 0L;
}
