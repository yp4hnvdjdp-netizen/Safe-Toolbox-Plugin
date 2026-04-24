#pragma once

#include <atomic>

struct MeterState
{
    void reset() noexcept
    {
        inputPeak.store (0.0f, std::memory_order_relaxed);
        outputPeak.store (0.0f, std::memory_order_relaxed);
        gainReductionDb.store (0.0f, std::memory_order_relaxed);
        clipWarning.store (0.0f, std::memory_order_relaxed);
    }

    std::atomic<float> inputPeak { 0.0f };
    std::atomic<float> outputPeak { 0.0f };
    std::atomic<float> gainReductionDb { 0.0f };
    std::atomic<float> clipWarning { 0.0f };
};
