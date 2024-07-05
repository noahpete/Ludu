#pragma once

#include "ldpch.h"
#include "Core/Core.h"

#include <random>
#include <cstdint>

namespace Util {

	static std::vector<char> ReadFile(const std::string& filepath)
	{
		std::ifstream file{ filepath, std::ios::ate | std::ios::binary };

		if (!file.is_open())
			LD_CORE_ERROR("Failed to open file: {0}", filepath);

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}

	static uint64_t GenerateUUID()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<uint64_t> dis;
		return dis(gen);
	}

	static double GetTime()
	{
		return std::clock() / 100000.0f;
	}

}