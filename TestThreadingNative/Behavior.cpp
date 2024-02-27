#include "Behavior.h"

#include "FakeTranscriber.h"

#include <cassert>

Behavior::Behavior()
{
}

void Behavior::SetCallbacks( TranscriptionStartedCallback transcriptionStartedCallback,
                             TranscriptionProgressCallback transcriptionProgressCallback,
                             TranscriptionFinishedCallback transcriptionFinishedCallback )
{
   _transcriptionStartedCallback = transcriptionStartedCallback;
   _transcriptionProgressCallback = transcriptionProgressCallback;
   _transcriptionFinishedCallback = transcriptionFinishedCallback;
}

void Behavior::StartTranscribing()
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
}

void Behavior::OnTranscriptionStarted()
{
   if ( _transcriptionStartedCallback )
   {
      _transcriptionStartedCallback();
   }
}

void Behavior::OnTranscriptionCompleted()
{
   auto this_threadID = std::this_thread::get_id();
   assert( _threadId == this_threadID );

   //Do things that ought to run on main thread.

   if ( _transcriptionFinishedCallback )
   {
      _transcriptionFinishedCallback();
   }
}
