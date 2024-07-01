#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <limits>

#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>


#include "Core/Core.h"
#include "Core/Log.h"
#include "Core/Assert.h"
#include "Core/Util.h"

#ifdef LD_PLATFORM_WINDOWS
	#include <Windows.h>
#endif