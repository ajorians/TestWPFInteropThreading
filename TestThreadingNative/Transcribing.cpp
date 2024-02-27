#include "Transcribing.h"

#include "FakeTranscriber.h"

Transcribing::Transcribing()
{
}

void Transcribing::SetCallbacks( TranscriptionProgressCallback transcriptionProgressCallback )
{
   _transcriptionProgressCallback = transcriptionProgressCallback;
}

void Transcribing::StartTranscribing()
{
   _transcriber.reset();

   _transcriber = std::make_unique<FakeTranscriber>( [this]( int progress )
   {
      _transcriptionProgressCallback( progress );
   } );
}
