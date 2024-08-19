#pragma once
#ifndef LOGGER_H
#define LOGGER_H



#include <iostream>
#include <fstream>
#include <string>
#include <mutex>


enum class LogLevel
{
    INFO,
    WARNING,
    FATAL_ERROR,
};


class AdvancedLogger {
public:

    AdvancedLogger(const std::string& filename = "");
    ~AdvancedLogger();

    void SetLogLevel(LogLevel level);
    void Log(LogLevel level, const std::string& message);

private:

    std::string LogLevelToString(LogLevel level) const;
    void LogToConsole(const std::string& message);
    void LogToFile(const std::string& message);

    LogLevel m_logLevel;
    std::ofstream m_fileStream;
    std::mutex m_mutex;
};

inline AdvancedLogger::AdvancedLogger(const std::string& filename)
    : m_logLevel(LogLevel::INFO) {
    if (!filename.empty()) {
        m_fileStream.open(filename, std::ios::app);
        if (!m_fileStream.is_open()) {
            LogToConsole("Failed to open log file: " + filename);
        }
    }
}

inline AdvancedLogger::~AdvancedLogger() {
    if (m_fileStream.is_open()) {
        m_fileStream.close();
    }
}

inline void AdvancedLogger::SetLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_logLevel = level;
}

inline void AdvancedLogger::Log(LogLevel level, const std::string& message) {
    if (level >= m_logLevel) {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::string fullMessage = LogLevelToString(level) + ": " + message;
        LogToConsole(fullMessage);
        if (m_fileStream.is_open()) {
            LogToFile(fullMessage);
        }
    }
}

inline std::string AdvancedLogger::LogLevelToString(LogLevel level) const {
    switch (level) {
    case LogLevel::INFO: return "INFO";
    case LogLevel::WARNING: return "WARNING";
    case LogLevel::FATAL_ERROR: return "ERROR";
    default: return "UNKNOWN";
    }
}

inline void AdvancedLogger::LogToConsole(const std::string& message) {
    std::cout << message << std::endl;
}

inline void AdvancedLogger::LogToFile(const std::string& message) {
    if (m_fileStream.is_open()) {
        m_fileStream << message << std::endl;
    }
}

#endif // LOGGER_H
