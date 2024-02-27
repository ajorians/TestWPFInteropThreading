#pragma once

#include "TranscriptionProgressEventArgs.h"

#include <TestThreadingNative/Behavior.h>

#include <functional>

namespace MyInterop
{
   public ref class Interop
   {
   public:
      Interop();

      void StartTranscription();

      virtual event System::EventHandler^ TranscriptionStarted;
      virtual event System::EventHandler<MyInterop::TranscriptionProgressEventArgs^>^ TranscriptionProgress;
      virtual event System::EventHandler^ TranscriptionFinished;

   private:
      Behavior* _transcribing;

      void TranscriptionStartedHandler();
      void TranscriptionProgressHandler( int progress );
      void TranscriptionFinishedHandler();

      delegate void TranscriptionStartedDelegate();
      TranscriptionStartedDelegate^ _transcriptionStartedDelegate = gcnew TranscriptionStartedDelegate( this, &Interop::TranscriptionStartedHandler );

      delegate void TranscriptionProgressDelegate( int progress );
      TranscriptionProgressDelegate^ _transcriptionProgressDelegate = gcnew TranscriptionProgressDelegate( this, &Interop::TranscriptionProgressHandler );

      delegate void TranscriptionFinishedDelegate();
      TranscriptionFinishedDelegate^ _transcriptionFinishedDelegate = gcnew TranscriptionFinishedDelegate( this, &Interop::TranscriptionFinishedHandler );
   };
}

