#pragma once

#include "TranscriptionProgressEventArgs.h"

#include <TestThreadingNative/Transcribing.h>

#include <functional>

namespace MyInterop
{
   public ref class Interop
   {
   public:
      Interop();

      void StartTranscription();

      virtual event System::EventHandler<MyInterop::TranscriptionProgressEventArgs^>^ TranscriptionProgress;

   private:
      Transcribing* _transcribing;

      void TranscriptionProgressHandler( int progress );

      delegate void TranscriptionProgressDelegate( int progress );
      TranscriptionProgressDelegate^ _transcriptionProgressDelegate = gcnew TranscriptionProgressDelegate( this, &Interop::TranscriptionProgressHandler );
   };
}

