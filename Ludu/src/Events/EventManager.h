#pragma once

//#include "ldpch.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include <typeindex>

namespace Ludu
{
    // adapted from https://gist.github.com/Jgb14002/44716ad59c9654ad08b59abbf1c45b40

    class EventDispatcher
    {
    public:
        virtual ~EventDispatcher() = default;
        inline void Dispatch(Event& e) const
        {
            InvokeHandler(e);
        }

    private:
        virtual void InvokeHandler(Event& e) const = 0;
    };

    template <typename Receiver, typename T>
    class EventHandler : public EventDispatcher
    {
    public:
        using HandlerFunc = void(Receiver::*)(T&);

        EventHandler(Receiver* receiver, HandlerFunc handlerFunc)
            : m_Receiver(receiver), m_HandlerFunc(handlerFunc) {}

        virtual void InvokeHandler(Event& e) const override
        {
            (m_Receiver->*m_HandlerFunc)(static_cast<T&>(e));
        }

        bool operator==(const EventHandler<Receiver, T>& rhs) const
        {
            return (m_Receiver == rhs.m_Receiver) && (m_HandlerFunc == rhs.m_HandlerFunc);
        }

        bool operator==(Receiver* rhs) const
        {
            return m_Receiver == rhs;
        }

    private:
        Receiver* m_Receiver;
        HandlerFunc m_HandlerFunc;
    };

    class EventManager
    {
    public:
        EventManager() = default;
        ~EventManager() = default;

        template <typename T>
        static void Publish(T&& event)
        {
            auto entry = s_Subscribers.find(typeid(T));
            if (entry != s_Subscribers.end())
            {
                for (const auto& handler : entry->second)
                {
                    if (event.Handled)
                        return;
                    handler->Dispatch(event);
                }
            }

            if (event.GetEventType() == EventType::MouseMove) return;

            LD_CORE_INFO("Published event " + event.ToString());
        }

        template <typename Receiver, typename T>
        static void Subscribe(Receiver* receiver, void (Receiver::* handlerFunc)(T&))
        {
            std::vector<Scope<EventDispatcher>>& handlers = s_Subscribers[typeid(T)];
            handlers.emplace_back(CreateScope<EventHandler<Receiver, T>>(receiver, handlerFunc));
        }

        template <typename Receiver, typename T>
        static void Unsubscribe(Receiver* receiver, void (Receiver::* handlerFunc)(T&))
        {
            // auto entry = s_Subscribers.find(typeid(T));
            // if (entry != s_Subscribers.end())
            // {
            //     const EventHandler<Receiver, T> tmp = {receiver, handlerFunc};
            //     std::vector<Scope<EventDispatcher>> &handlers = entry->second;
            //     handlers.erase(std::remove_if(handlers.begin(), handlers.end(), [&](const auto &handler)
            //                                   {
            //         using T = EventHandler<Receiver, T>;
            //         return *static_cast<T*>(handler.get()) == tmp; }),
            //                    handlers.end());
            // }
        }

    private:
        static std::unordered_map<std::type_index, std::vector<Scope<EventDispatcher>>> s_Subscribers;
    };
}