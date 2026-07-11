#pragma once

#include <chrono>

class Time
{
private:
    friend void UpdateTime();

    static inline std::chrono::nanoseconds startTime = std::chrono::high_resolution_clock::now().time_since_epoch();
    static inline std::chrono::nanoseconds lastFrameTime = startTime;

    static inline float totalTime = 0.0f;
    static inline float deltaTime = 0.0f;

public:
    static float GetTotalTime() { return totalTime; }
    static float GetDeltaTime() { return deltaTime; }

private:
    void static Update()
    {
        using namespace std::chrono;

        deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch() - lastFrameTime) / 1.0s);

        if (deltaTime > 0.1f)
            deltaTime = 0.1f;

        totalTime += deltaTime;
        lastFrameTime = std::chrono::high_resolution_clock::now().time_since_epoch();
    }
};