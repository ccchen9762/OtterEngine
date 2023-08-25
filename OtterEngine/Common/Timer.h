#pragma once

#include "pch.h"

// query performance counter (QPC)
// API for high-resolution time stamps or measure time intervals

// calculation: performance counters between last frame and this frame / counters per second (frquency)

class Timer {
public:
    Timer() : m_framesThisInterval(0u), m_framesPerSecond(0.0), m_updateInterval(1.0) {
        // The frequency of the performance counter is fixed at system boot and is consistent across all processors
        // in counts per second
        if (!QueryPerformanceFrequency(&m_qpcFrequency))
            throw std::exception("QueryPerformanceFrequency");

        // Retrieves the current value of the performance counter
        // which is a high resolution (<1us) time stamp
        if (!QueryPerformanceCounter(&m_startTime))
            throw std::exception("QueryPerformanceCounter");

        m_currentTime = m_startTime;
        m_lastUpdate = m_startTime;
        m_elapsedTime = m_startTime;
    };

    double GetElapsedTimeSecond() { return m_elapsedTime.QuadPart / static_cast<double>(m_qpcFrequency.QuadPart); }
    double GetElapsedTimeMilliSecond() { return m_elapsedTime.QuadPart * 1000 / static_cast<double>(m_qpcFrequency.QuadPart); }
    double GetFramesPerSecond() { return m_framesPerSecond; }

    double Update() {
        LARGE_INTEGER previousFrame = m_currentTime;
        if (!QueryPerformanceCounter(&m_currentTime))
            throw std::exception("QueryPerformanceCounter");

        m_elapsedTime.QuadPart = static_cast<uint64_t>(m_currentTime.QuadPart - m_startTime.QuadPart);
        
        // update frames per second
        ++m_framesThisInterval;
        if (m_currentTime.QuadPart - m_lastUpdate.QuadPart >= m_qpcFrequency.QuadPart * m_updateInterval) {
            m_framesPerSecond = m_framesThisInterval / (static_cast<double>(m_currentTime.QuadPart - m_lastUpdate.QuadPart) / m_qpcFrequency.QuadPart);

            // reset 
            m_lastUpdate = m_currentTime;
            m_framesThisInterval = 0;
        }

        return (m_currentTime.QuadPart - previousFrame.QuadPart) / static_cast<double>(m_qpcFrequency.QuadPart);
    }

    void Reset() {
        if (!QueryPerformanceCounter(&m_startTime))
            throw std::exception("QueryPerformanceCounter");
    }

private:
    // time measurement
    LARGE_INTEGER m_qpcFrequency;
    LARGE_INTEGER m_startTime;
    LARGE_INTEGER m_currentTime;
    LARGE_INTEGER m_elapsedTime;

    // frames per second measurement
    LARGE_INTEGER m_lastUpdate;
    uint64_t m_framesThisInterval;
    double m_framesPerSecond;
    double m_updateInterval;
};