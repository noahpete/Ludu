#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Core/Core.h"
#include "Core/Log.h"
#include "Core/Assert.h"

#ifdef LD_PLATFORM_WINDOWS
	#include <Windows.h>
#endif