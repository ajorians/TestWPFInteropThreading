#pragma once

#include <functional>

using TranscriptionProgressCallback = std::function<void( int progress )>;

class Transcribing
{
public:
   Transcribing();

   void SetCallbacks( TranscriptionProgressCallback transcriptionProgressCallback );

   void StartTranscribing();

private:
   TranscriptionProgressCallback _transcriptionProgressCallback;
};
