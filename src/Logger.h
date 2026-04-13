#pragma once

#include "spdlog/common.h"
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

class Logger {
public:
  struct Config {
    spdlog::level::level_enum log_level;
  };

  static void init(Config config);
  static void init();

  static void shutdown();
};
