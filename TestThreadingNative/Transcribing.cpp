#include "Transcribing.h"

Transcribing::Transcribing()
{

}

void Transcribing::SetCallbacks( TranscriptionProgressCallback transcriptionProgressCallback )
{
   _transcriptionProgressCallback = transcriptionProgressCallback;
}

void Transcribing::StartTranscribing()
{
   //Start Thread

   _transcriptionProgressCallback( 45 );
}
