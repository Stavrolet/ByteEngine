#pragma once

#ifdef BE_WINDOWS_MAIN
struct HINSTANCE__;
using HINSTANCE = HINSTANCE__*;
#endif

namespace ByteEngine
{
    template <typename T>
    class Singleton
    {
        friend class Application;

#ifdef BE_WINDOWS_MAIN
        friend int __stdcall ::WinMain(HINSTANCE, HINSTANCE, char*, int);
#endif

    private:
        static inline T* instance;

    public:
        Singleton(const Singleton&) = delete;
        virtual ~Singleton() { instance = nullptr; }

        Singleton& operator=(const Singleton&) = delete;

        static T& GetInstance()
        {
            return *instance;
        }

    protected:
        Singleton() = default;
        Singleton(Singleton&&) = default;

        Singleton& operator=(Singleton&&) = default;

    private:
        static void SetInstance(T* newInstance)
        {
            instance = newInstance;
        }
    };
}