#include "platform/logger/logger.h"

#include <spdlog/sinks/stdout_sinks.h>

void registerLoggers() {
  auto console = spdlog::stdout_color_mt(LOGGER_DEFAULT);
}