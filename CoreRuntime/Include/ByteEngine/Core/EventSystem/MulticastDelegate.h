#pragma once

#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>
#include <type_traits>
#include <vector>

#include "ByteEngine/Detail/Core/EventSystem/Subscriptions.h"
#include "ByteEngine/Primitives.h"

namespace ByteEngine::EventSystem
{
    template<typename... Args>
    class MulticastDelegate
    {
    public:
        using FunctionType = void(*)(Args...);

    private:
        using SubscriptionItem = std::pair<SubscriptionHandle, std::unique_ptr<Subcription<void, Args...>>>;

        std::vector<SubscriptionItem> subscriptions;

        std::vector<SubscriptionItem> pendingSubscriptions;
        std::vector<SubscriptionHandle> pendingRemovals;

        bool invoked = false;

        int32 invokesCount = 0;

    public:
        MulticastDelegate() = default;

        MulticastDelegate(const MulticastDelegate&) = delete;
        MulticastDelegate& operator=(const MulticastDelegate&) = delete;

        MulticastDelegate(MulticastDelegate&&) = default;
        MulticastDelegate& operator=(MulticastDelegate&&) = default;

        SubscriptionHandle SubscribeStatic(FunctionType func)
        {
            if (invoked)
                return SubscribePendingInternal(std::make_unique<StaticSubcription<void, Args...>>(func));
            else
                return SubscribeInternal(std::make_unique<StaticSubcription<void, Args...>>(func));
        }

        template<typename InstanceT>
        SubscriptionHandle SubscribeRawPointer(InstanceT* instance, void(InstanceT::* method)(Args...))
        {
            if (invoked)
                return SubscribePendingInternal(std::make_unique<RawPtrSubcription<InstanceT, void, Args...>>(instance, method));
            else
                return SubscribeInternal(std::make_unique<RawPtrSubcription<InstanceT, void, Args...>>(instance, method));
        }

        template<typename InstanceT>
        SubscriptionHandle SubscribeSmartPointer(const std::shared_ptr<InstanceT>& instance, void(InstanceT::* method)(Args...))
        {
            if (invoked)
                return SubscribePendingInternal(std::make_unique<SmartPtrSubcription<InstanceT, void, Args...>>(instance, method));
            else
                return SubscribeInternal(std::make_unique<SmartPtrSubcription<InstanceT, void, Args...>>(instance, method));
        }

        template<typename LambdaT>
        SubscriptionHandle SubscribeLambda(LambdaT&& lambda)
        {
            if (invoked)
                return SubscribePendingInternal(std::make_unique<LambdaSubcription<std::decay_t<LambdaT>, Args...>>(std::forward<LambdaT>(lambda)));
            else
                return SubscribeInternal(std::make_unique<LambdaSubcription<std::decay_t<LambdaT>, void, Args...>>(std::forward<LambdaT>(lambda)));
        }

        void Unsubscribe(SubscriptionHandle handle)
        {
            assert(handle != 0 && "Subscription handle must not be equal to 0.");

            if (invoked)
                pendingRemovals.emplace_back(handle);
            else
                subscriptions.erase(handle);
        }

        void UnsubscribeObject(const void* objectToUnsubscribe)
        {
            assert(objectToUnsubscribe != nullptr && "Object pointer cannot be null.");

            if (invoked)
            {
                for (const auto& [handle, subscription] : subscriptions)
                {
                    if (subscription->GetOwner() == objectToUnsubscribe)
                        pendingRemovals.emplace_back(handle);
                }
            }
            else
            {
                std::erase_if(subscriptions, [=](const SubscriptionItem& item)
                {
                    return item.second->GetOwner() == objectToUnsubscribe;
                });
            }
        }

        void Clear()
        {
            subscriptions.clear();
        }

        void Invoke(Args... args)
        {
            invokesCount++;
            invoked = true;
            std::erase_if(subscriptions, [](const SubscriptionItem& item) { return item.second->IsExpired(); });

            for (const auto& [handle, subscription] : subscriptions)
            {
                if (subscription)
                    subscription->Invoke(std::forward<Args>(args)...);
            }

            invoked = false;

            for (SubscriptionHandle handle : pendingRemovals)
                std::erase_if(subscriptions, [=](const SubscriptionItem& item) { return item.first == handle; });
            pendingRemovals.clear();

            subscriptions.insert(subscriptions.end(), std::make_move_iterator(pendingSubscriptions.begin()), std::make_move_iterator(pendingSubscriptions.end()));
            pendingSubscriptions.clear();

            if (invokesCount > 50)
            {
                invokesCount = 0;
                subscriptions.shrink_to_fit();

                if (pendingSubscriptions.capacity() > 10)
                    subscriptions.reserve(10);

                if (pendingRemovals.capacity() > 10)
                    pendingRemovals.reserve(10);
            }
        }

        bool HasSubscribers() const
        {
            return !subscriptions.empty();
        }

        bool IsObjectSubscribed(const void* object) const
        {
            return std::any_of(subscriptions.begin(), subscriptions.end(), [=](const SubscriptionItem& item)
            {
                return item.second->GetOwner() == object;
            });
        }

    private:
        static SubscriptionHandle GenerateUniqueId()
        {
            static SubscriptionHandle idCounter = 1;
            return idCounter++;
        }

        SubscriptionHandle SubscribeInternal(std::unique_ptr<Subcription<void, Args...>>&& instance)
        {
            SubscriptionHandle handle = GenerateUniqueId();
            subscriptions.emplace_back(handle, std::move(instance));
            return handle;
        }

        SubscriptionHandle SubscribePendingInternal(std::unique_ptr<Subcription<void, Args...>>&& instance)
        {
            SubscriptionHandle handle = GenerateUniqueId();
            pendingSubscriptions.emplace_back(handle, std::move(instance));
            return handle;
        }
    };

    using MulticastDelegateVoid = MulticastDelegate<>;
}