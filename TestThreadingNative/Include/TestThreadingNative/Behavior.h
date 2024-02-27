#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <thread>

class FakeTranscriber;

using TranscriptionStartedCallback = std::function<void()>;
using TranscriptionProgressCallback = std::function<void(int progress)>;
using TranscriptionFinishedCallback = std::function<void()>;

class Behavior
{
public:
   Behavior();

   void SetCallbacks( TranscriptionStartedCallback transcriptionStartedCallback,
                      TranscriptionProgressCallback transcriptionProgressCallback,
                      TranscriptionFinishedCallback transcriptionFinishedCallback );

   void StartTranscribing();

private:
   void OnTranscriptionStarted();
   void OnTranscriptionCompleted();

   std::thread::id _threadId;

   TranscriptionStartedCallback _transcriptionStartedCallback;
   TranscriptionProgressCallback _transcriptionProgressCallback;
   TranscriptionFinishedCallback _transcriptionFinishedCallback;

   std::unique_ptr<FakeTranscriber> _transcriber;
};
