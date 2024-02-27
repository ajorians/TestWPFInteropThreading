#pragma once

#include <functional>
#include <memory>

#include "GenericTimer.h"

class FakeTranscriber;

using TranscriptionStartedCallback = std::function<void()>;
using TranscriptionProgressCallback = std::function<void(int progress)>;
using TranscriptionFinishedCallback = std::function<void()>;

class BehaviorImpl;

class Behavior
{
public:
   Behavior();

   void SetCallbacks( TranscriptionStartedCallback transcriptionStartedCallback,
                      TranscriptionProgressCallback transcriptionProgressCallback,
                      TranscriptionFinishedCallback transcriptionFinishedCallback );

   void StartTranscribing();

private:
   std::unique_ptr<BehaviorImpl> _impl;
};
