#include "Interop.h"
#include "DelegateToStdFunction.h"

using namespace MyInterop;

Interop::Interop()
{
   _transcribing = new Transcribing();

   _transcribing->SetCallbacks( {},
                                InteropHelper::DelegateToStdFunction<TranscriptionProgressCallback>( _transcriptionProgressDelegate ),
                                {} );
}

void Interop::StartTranscription()
{
   _transcribing->StartTranscribing();
}

void Interop::TranscriptionProgressHandler( int progress )
{
   MyInterop::TranscriptionProgressEventArgs^ args = gcnew MyInterop::TranscriptionProgressEventArgs();
   args->Progress = progress;

   TranscriptionProgress( this, args );
}