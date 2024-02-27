#include "Transcribing.h"

#include "FakeTranscriber.h"

Transcribing::Transcribing()
{
}

void Transcribing::SetCallbacks( TranscriptionStartedCallback transcriptionStartedCallback,
                                 TranscriptionProgressCallback transcriptionProgressCallback,
                                 TranscriptionFinishedCallback transcriptionFinishedCallback )
{
   _transcriptionStartedCallback = transcriptionStartedCallback;
   _transcriptionProgressCallback = transcriptionProgressCallback;
   _transcriptionFinishedCallback = transcriptionFinishedCallback;
}

void Transcribing::StartTranscribing()
{
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

void Transcribing::OnTranscriptionStarted()
{
   if ( _transcriptionStartedCallback )
   {
      _transcriptionStartedCallback();
   }
}

void Transcribing::OnTranscriptionCompleted()
{
   if ( _transcriptionFinishedCallback )
   {
      //... 

      _transcriptionFinishedCallback();
   }
}
