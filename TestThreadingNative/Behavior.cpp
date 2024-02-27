#include "Behavior.h"

#include "FakeTranscriber.h"

#include <cassert>
#include <chrono>
#include <optional>
#include <thread>
#include <vector>
#include <string>
#include <mutex>

using namespace std::chrono_literals;

class BehaviorImpl
{
public:
   BehaviorImpl()
      : m_timer( [this]() { OnTimerElapsed(); }, 500ms )
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
               OnTranscriptionStarted();
            }
            else
            {
               OnTranscriptionCompleted();
            }
         }

      _transcriptionProgressCallback( progress );

      return true;
      } );

      m_timer.StartTimer();
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
      std::unique_lock myLock( _messageMutex );

      _messages.push_back( "Completed :)" );

      //auto this_threadID = std::this_thread::get_id();
      //assert( _threadId == this_threadID );

      ////Do things that ought to run on main thread.

      //if ( _transcriptionFinishedCallback )
      //{
      //   _transcriptionFinishedCallback();
      //}
   }

   void OnTimerElapsed()
   {
      auto this_threadID = std::this_thread::get_id();
      assert( _threadId == this_threadID );

      std::unique_lock myLock( _messageMutex );

      for ( const auto& message : _messages )
      {
         if ( _transcriptionFinishedCallback )
         {
            _transcriptionFinishedCallback();
         }
      }

      _messages.clear();
   }

   std::thread::id _threadId;

   TranscriptionStartedCallback _transcriptionStartedCallback;
   TranscriptionProgressCallback _transcriptionProgressCallback;
   TranscriptionFinishedCallback _transcriptionFinishedCallback;

   std::unique_ptr<FakeTranscriber> _transcriber;
   std::vector<std::string> _messages;

   std::mutex _messageMutex;
   GenericTimer m_timer;
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
