#pragma once

#include <functional>
#include <memory>

class FakeTranscriberImpl;

class FakeTranscriber
{
public:
   FakeTranscriber( std::function<void(int progress)> progressCallback );
   ~FakeTranscriber();

private:
   std::unique_ptr<FakeTranscriberImpl> _impl;
};
