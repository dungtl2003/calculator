#include "Logger.h"
#include <iostream>

void Logger::init() {
  Logger::init(Config{
      .log_level = spdlog::level::debug,
  });
}

void Logger::init(Config config) {
  try {
    // 1. Initialize the async thread pool
    // 8192 items in the lock-free queue, 1 dedicated background thread
    spdlog::init_thread_pool(8192, 1);

    // 2. Create sinks (destinations for the log messages)
    // _mt denotes multi-thread safe sinks
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        "logs/performance.log", true);

    // 3. Combine sinks into a single asynchronous logger
    std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};

    // Define the queue overflow policy.
    // 'block' pauses the caller if the queue is full to ensure no dropped
    // logs. 'overrun_oldest' drops the oldest logs to ensure the hot-path
    // never blocks.
    auto logger = std::make_shared<spdlog::async_logger>(
        "async_logger", sinks.begin(), sinks.end(), spdlog::thread_pool(),
        spdlog::async_overflow_policy::overrun_oldest);

    // 4. Set as the global default logger
    spdlog::register_logger(logger);
    spdlog::set_default_logger(logger);

    // 5. Configure log level and formatting pattern
    spdlog::set_level(config.log_level);

    // Pattern breakdown: [Timestamp] [Log Level (colored)] [Thread ID]
    // Message
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] %v");

  } catch (const spdlog::spdlog_ex &ex) {
    std::cerr << "Log initialization failed: " << ex.what() << std::endl;
  }
}

void Logger::shutdown() {
  // Ensure all remaining messages in the queue are flushed before exit
  spdlog::shutdown();
}
