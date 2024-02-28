#include "Dispatcher.h"

#define WM_APP_DISPATCHER_MESSAGE (WM_USER + 1234)

#define NEVER_TRANSLATE(x) L ## x

Dispatcher::Dispatcher()
{
   //Message-only window: https://docs.microsoft.com/en-us/windows/win32/winmsg/window-features?redirectedfrom=MSDN#message-only-windows
   const wchar_t* className = NEVER_TRANSLATE( "MyDispatcher" );
   WNDCLASSEX wx
   {
      .cbSize = sizeof( WNDCLASSEX ),
      .lpfnWndProc = Dispatcher::WindowProc,
      .hInstance = GetModuleHandle( NULL ),
      .lpszClassName = className
   };
   (void)RegisterClassEx( &wx );

   _hwnd = CreateWindowEx( 0, className, NEVER_TRANSLATE( "Dispatcher Message-Only Wnd :)" ), 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL );
   SetWindowLongPtr( _hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( this ) );
}

void Dispatcher::CallOnMainThread( std::function<void()> func )
{
   _functionsToCallOnMainThread.emplace_back( func );

   ::PostMessage( _hwnd, WM_APP_DISPATCHER_MESSAGE, 0, 0 );
}

/*static*/
LRESULT CALLBACK Dispatcher::WindowProc( HWND   hwnd,
                                           UINT   uMsg,
                                           WPARAM wParam,
                                           LPARAM lParam )
{
   if ( Dispatcher* pSelf = reinterpret_cast<Dispatcher*>( GetWindowLongPtr( hwnd, GWLP_USERDATA ) );
        pSelf != nullptr )
   {
      return pSelf->WndProc( uMsg, wParam, lParam );
   }

   return ::DefWindowProc( hwnd, uMsg, wParam, lParam );
}

LRESULT Dispatcher::WndProc( UINT   uMsg,
                               WPARAM wParam,
                               LPARAM lParam )
{
   if ( uMsg == WM_APP_DISPATCHER_MESSAGE )
   {
      for ( const auto& func : _functionsToCallOnMainThread )
      {
         func();
      }
      _functionsToCallOnMainThread.clear();

      return 1L;
   }

   return 0L;
}
