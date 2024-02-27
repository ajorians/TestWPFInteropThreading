#pragma once

#include <functional>
#include <memory>
#include <optional>

class FakeTranscriberImpl;

class FakeTranscriber
{
public:
   FakeTranscriber( std::function<bool(int progress, std::optional<bool> status)> progressCallback );
   ~FakeTranscriber();

private:
   std::unique_ptr<FakeTranscriberImpl> _impl;
};
