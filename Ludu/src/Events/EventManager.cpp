#include "ldpch.h"
#include "Events/EventManager.h"

namespace Ludu {

    std::unordered_map<std::type_index, std::vector<Scope<EventDispatcher>>> EventManager::s_Subscribers;

}