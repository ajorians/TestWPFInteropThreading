#include "Interop.h"
#include "DelegateToStdFunction.h"

using namespace MyInterop;

Interop::Interop()
{
   _transcribing = new Transcribing();

   _transcribing->SetCallbacks( InteropHelper::DelegateToStdFunction<TranscriptionStartedCallback>( _transcriptionStartedDelegate ),
                                InteropHelper::DelegateToStdFunction<TranscriptionProgressCallback>( _transcriptionProgressDelegate ),
                                InteropHelper::DelegateToStdFunction<TranscriptionFinishedCallback>( _transcriptionFinishedDelegate ) );
}

void Interop::StartTranscription()
{
   _transcribing->StartTranscribing();
}

void Interop::TranscriptionStartedHandler()
{
   TranscriptionStarted( this, System::EventArgs::Empty );
}

void Interop::TranscriptionProgressHandler( int progress )
{
   MyInterop::TranscriptionProgressEventArgs^ args = gcnew MyInterop::TranscriptionProgressEventArgs();
   args->Progress = progress;

   TranscriptionProgress( this, args );
}

void Interop::TranscriptionFinishedHandler()
{
   TranscriptionFinished( this, System::EventArgs::Empty );
}
