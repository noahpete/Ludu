#pragma once

#include "Core/Core.h"
#include "Core/Log.h"

// TODO: create Asserts and add preprocessor directive
#ifdef LD_ENABLE_ASSERTS
#define LD_ASSERT(...)
#define LD_CORE_ASSERT(...)
#else
#define LD_ASSERT(...)
#define LD_CORE_ASSERT(...)
#endif