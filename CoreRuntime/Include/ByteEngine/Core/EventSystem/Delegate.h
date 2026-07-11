#pragma once

#include "ByteEngine/Detail/Core/EventSystem/Subscriptions.h"

namespace ByteEngine::EventSystem
{
    template<typename Ret, typename... Args>
    class Delegate
    {
    public:
        using FunctionType = Ret(*)(Args...);

    private:
        std::unique_ptr<Subcription<Ret, Args...>> subscription;

    public:
        Delegate() = default;

        Delegate(const Delegate&) = delete;
        Delegate& operator=(const Delegate&) = delete;

        Delegate(Delegate&&) = default;
        Delegate& operator=(Delegate&&) = default;

        void SubscribeStatic(FunctionType func)
        {
            subscription = std::make_unique<StaticSubcription<Ret, Args...>>(func);
        }

        template<typename InstanceT>
        void SubscribeRawPointer(InstanceT* instance, Ret(InstanceT::* method)(Args...))
        {
            subscription = std::make_unique<RawPtrSubcription<InstanceT, Ret, Args...>>(instance, method);
        }

        template<typename InstanceT>
        void SubscribeSmartPointer(const std::shared_ptr<InstanceT>& instance, Ret(InstanceT::* method)(Args...))
        {
            subscription = std::make_unique<SmartPtrSubcription<InstanceT, Ret, Args...>>(instance, method);
        }

        template<typename LambdaT>
        void SubscribeLambda(LambdaT&& lambda)
        {
            subscription = std::make_unique<LambdaSubcription<std::decay_t<LambdaT>, Ret, Args...>>(std::forward<LambdaT>(lambda));
        }

        void Unsubscribe()
        {
            subscription.reset();
        }

        Ret Invoke(Args... args)
        {
            if (subscription)
            {
                if constexpr (std::is_void_v<Ret>)
                    subscription->Invoke(std::forward<Args>(args)...);
                else
                    return subscription->Invoke(std::forward<Args>(args)...);
            }
            else
            {
                if constexpr (!std::is_void_v<Ret>)
                    return Ret();
            }
        }

        bool HasSubscriber() const { return static_cast<bool>(subscription); }
    };

    template<typename... Args>
    using DelegateVoid = Delegate<void, Args...>;

    template<typename Ret>
    using Delegate0 = Delegate<Ret>;

    using DelegateVoid0 = Delegate<void>;
}