#pragma once

#include <functional>
#include <memory>

class FakeTranscriber;

using TranscriptionProgressCallback = std::function<void( int progress )>;

class Transcribing
{
public:
   Transcribing();

   void SetCallbacks( TranscriptionProgressCallback transcriptionProgressCallback );

   void StartTranscribing();

private:
   TranscriptionProgressCallback _transcriptionProgressCallback;

   std::unique_ptr<FakeTranscriber> _transcriber;
};
