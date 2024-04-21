#pragma once
#include <string>
#include <cstdint>

//base class for all engine objects
namespace minty
{
	class EngineObj {
	public:
		static std::uint64_t gIdGen;
		std::uint64_t mId;
		std::string mData;
	};
}