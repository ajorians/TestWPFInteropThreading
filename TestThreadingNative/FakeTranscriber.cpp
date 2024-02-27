#include "FakeTranscriber.h"

#include <thread>
#include <chrono>

using namespace std::chrono_literals;

class FakeTranscriberImpl
{
public:
   FakeTranscriberImpl( std::function<void( int progress )> progressCallback )
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
      for ( int i = 0; i <= 100; i++ )
      {
         if ( _exit )
         {
            return;
         }

         std::this_thread::sleep_for ( 50ms );
         _progressCallback( i );
      }
   }

   std::function<void( int progress )> _progressCallback;

   std::thread _transcriptionThread;

   bool _exit = false;;
};

FakeTranscriber::FakeTranscriber( std::function<void( int progress )> progressCallback )
{
   _impl = std::make_unique<FakeTranscriberImpl>( progressCallback );
}

FakeTranscriber::~FakeTranscriber()
{
}
