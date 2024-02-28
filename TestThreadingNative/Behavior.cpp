#include "Behavior.h"

#include "FakeTranscriber.h"

#include <cassert>
#include <chrono>
#include <optional>
#include <thread>
#include <vector>
#include <string>
#include <mutex>

#include "Dispatcher.h"

using namespace std::chrono_literals;

class BehaviorImpl
{
public:
   BehaviorImpl()
   {
   }

   void SetCallbacks( TranscriptionStartedCallback transcriptionStartedCallback,
                      TranscriptionProgressCallback transcriptionProgressCallback,
                      TranscriptionFinishedCallback transcriptionFinishedCallback )
   {
      _transcriptionStartedCallback = transcriptionStartedCallback;
      _transcriptionProgressCallback = transcriptionProgressCallback;
      _transcriptionFinishedCallback = transcriptionFinishedCallback;
   }

   void StartTranscribing()
   {
      _threadId = std::this_thread::get_id();

      _transcriber.reset();

      _transcriber = std::make_unique<FakeTranscriber>( [this]( int progress, std::optional<bool> status ) -> bool
      {
         if ( status.has_value() )
         {
            if ( status.value() )
            {
               _dispatcher.CallOnMainThread( [this]()
               {
                  OnTranscriptionStarted();
               } );
            }
            else
            {
               _dispatcher.CallOnMainThread( [this]()
               {
                  OnTranscriptionCompleted();
               } );
            }
         }

      _dispatcher.CallOnMainThread( [=, this]()
      {
         _transcriptionProgressCallback( progress );
      } );

      return true;
      } );
   }

private:
   void OnTranscriptionStarted()
   {
      if ( _transcriptionStartedCallback )
      {
         _transcriptionStartedCallback();
      }
   }

   void OnTranscriptionCompleted()
   {
      auto this_threadID = std::this_thread::get_id();
      assert( _threadId == this_threadID );

      //Do things that ought to run on main thread.

      if ( _transcriptionFinishedCallback )
      {
         _transcriptionFinishedCallback();
      }
   }

   std::thread::id _threadId;

   TranscriptionStartedCallback _transcriptionStartedCallback;
   TranscriptionProgressCallback _transcriptionProgressCallback;
   TranscriptionFinishedCallback _transcriptionFinishedCallback;

   std::unique_ptr<FakeTranscriber> _transcriber;
   std::vector<std::string> _messages;

   std::mutex _messageMutex;
   Dispatcher _dispatcher;
};

Behavior::Behavior()
{
   _impl = std::make_unique<BehaviorImpl>();
}

void Behavior::SetCallbacks( TranscriptionStartedCallback transcriptionStartedCallback,
                             TranscriptionProgressCallback transcriptionProgressCallback,
                             TranscriptionFinishedCallback transcriptionFinishedCallback )
{
   _impl->SetCallbacks( transcriptionStartedCallback, transcriptionProgressCallback, transcriptionFinishedCallback );
}

void Behavior::StartTranscribing()
{
   _impl->StartTranscribing();
}
