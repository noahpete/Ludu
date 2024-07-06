#include "ldpch.h"
#include "Core/Util.h"

#include <GLFW/glfw3.h>

float Util::GetTime()
{
	return glfwGetTime();
}
