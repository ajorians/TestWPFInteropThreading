#include "FakeTranscriber.h"

#include <thread>
#include <chrono>

using namespace std::chrono_literals;

class FakeTranscriberImpl
{
public:
   FakeTranscriberImpl( std::function<bool(int progress, std::optional<bool> status)> progressCallback )
      : _progressCallback( progressCallback )
      , _transcriptionThread( [this] { ThreadWorker(); } )
   {
   }

   ~FakeTranscriberImpl()
   {
      _exit = true;

      _transcriptionThread.join();
   }

private:
   void ThreadWorker()
   {
      _progressCallback( 0, true );
      //std::this_thread::sleep_for ( 50ms );

      for ( int i = 0; i <= 100; i++ )
      {
         if ( _exit )
         {
            return;
         }

         std::this_thread::sleep_for ( 10ms );
         _progressCallback( i, std::nullopt );
      }

      _progressCallback( 100, false );
   }

   std::function<bool(int progress, std::optional<bool> status)> _progressCallback;

   std::thread _transcriptionThread;

   bool _exit = false;;
};

FakeTranscriber::FakeTranscriber( std::function<bool(int progress, std::optional<bool> status)> progressCallback )
{
   _impl = std::make_unique<FakeTranscriberImpl>( progressCallback );
}

FakeTranscriber::~FakeTranscriber()
{
}
