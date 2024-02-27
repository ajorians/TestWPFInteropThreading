#pragma once

#include <functional>
#include <memory>
#include <optional>

class FakeTranscriber;

using TranscriptionStartedCallback = std::function<void()>;
using TranscriptionProgressCallback = std::function<void(int progress)>;
using TranscriptionFinishedCallback = std::function<void()>;

class Transcribing
{
public:
   Transcribing();

   void SetCallbacks( TranscriptionStartedCallback transcriptionStartedCallback,
                      TranscriptionProgressCallback transcriptionProgressCallback,
                      TranscriptionFinishedCallback transcriptionFinishedCallback );

   void StartTranscribing();

private:
   void OnTranscriptionStarted();
   void OnTranscriptionCompleted();

   TranscriptionStartedCallback _transcriptionStartedCallback;
   TranscriptionProgressCallback _transcriptionProgressCallback;
   TranscriptionFinishedCallback _transcriptionFinishedCallback;

   std::unique_ptr<FakeTranscriber> _transcriber;
};
