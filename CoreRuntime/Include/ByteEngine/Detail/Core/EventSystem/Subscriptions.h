#pragma once

#include <memory>
#include <type_traits>
#include <utility>

#include "ByteEngine/Primitives.h"

namespace ByteEngine::EventSystem
{
    using SubscriptionHandle = uint64;

    template<typename Ret, typename... Args>
    class Subcription
    {
    public:
        virtual ~Subcription() = default;
        virtual Ret Invoke(Args... args) const = 0;
        virtual bool IsExpired() const { return false; }
        virtual const void* GetOwner() const { return nullptr; }
    };

    template<typename Ret, typename... Args>
    class StaticSubcription : public Subcription<Ret, Args...>
    {
    private:
        using FuncPtr = Ret(*)(Args...);
        FuncPtr function;

    public:
        explicit StaticSubcription(FuncPtr function)
            : function(function)
        { }

        Ret Invoke(Args... args) const override
        {
            if constexpr (std::is_void_v<Ret>)
                function(std::forward<Args>(args)...);
            else
                return function(std::forward<Args>(args)...);
        }
    };

    template<typename InstanceT, typename Ret, typename... Args>
    class RawPtrSubcription : public Subcription<Ret, Args...>
    {
    private:
        using MethodPtr = Ret(InstanceT::*)(Args...);
        InstanceT* instance;
        MethodPtr methodPtr;

    public:
        RawPtrSubcription(InstanceT* InObj, MethodPtr InMethod)
            : instance(InObj), methodPtr(InMethod)
        { }

        Ret Invoke(Args... args) const override
        {
            if constexpr (std::is_void_v<Ret>)
                (instance->*methodPtr)(std::forward<Args>(args)...);
            else
                return (instance->*methodPtr)(std::forward<Args>(args)...);
        }

        const void* GetOwner() const override { return instance; }
    };

    template<typename InstanceT, typename Ret, typename... Args>
    class SmartPtrSubcription : public Subcription<Ret, Args...>
    {
    private:
        using MethodPtr = Ret(InstanceT::*)(Args...);

        std::weak_ptr<InstanceT> objInstance;
        MethodPtr methodPtr;

    public:
        SmartPtrSubcription(const std::shared_ptr<InstanceT>& instance, MethodPtr method)
            : objInstance(instance), methodPtr(method)
        { }

        Ret Invoke(Args... args) const override
        {
            std::shared_ptr<InstanceT> pinned = objInstance.lock();

            if (pinned)
            {
                if constexpr (std::is_void_v<Ret>)
                    (pinned.get()->*methodPtr)(std::forward<Args>(args)...);
                else
                    return (pinned.get()->*methodPtr)(std::forward<Args>(args)...);
            }
            else
            {
                if constexpr (!std::is_void_v<Ret>)
                    return Ret();
            }
        }

        bool IsExpired() const override
        {
            return objInstance.expired();
        }

        const void* GetOwner() const override
        {
            std::shared_ptr<InstanceT> pinned = objInstance.lock();

            if (pinned)
                return pinned.get();
            else
                return nullptr;
        }
    };

    template<typename LambdaT, typename Ret, typename... Args>
    class LambdaSubcription : public Subcription<Ret, Args...>
    {
    private:
        LambdaT lambdaInstance;

    public:
        explicit LambdaSubcription(LambdaT&& lamda)
            : lambdaInstance(std::forward<LambdaT>(lamda))
        { }

        Ret Invoke(Args... args) const override
        {
            if constexpr (std::is_void_v<Ret>)
                lambdaInstance(std::forward<Args>(args)...);
            else
                return lambdaInstance(std::forward<Args>(args)...);
        }
    };
}