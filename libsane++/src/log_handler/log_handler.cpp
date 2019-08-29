#include <log_handler/log_handler.hpp>

// 3rd Party
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace sane {
    std::map<std::string, std::shared_ptr<spdlog::sinks::basic_file_sink_mt>> LogHandler::sinks =
            std::map<std::string, std::shared_ptr<spdlog::sinks::basic_file_sink_mt>>();

    LogHandler::LogHandler() = default;

    std::shared_ptr<spdlog::logger> LogHandler::createLogger(const std::string &t_facility,
            const std::string &t_logFile) {
        std::shared_ptr<spdlog::logger> logger;

        try {
            if (sinks.find(t_logFile) == sinks.end()) {
                // If given logfile sink doesn't exist, create it.
                sinks[t_logFile] = std::make_shared<spdlog::sinks::basic_file_sink_mt>(t_logFile);
            }

            logger = std::make_shared<spdlog::logger>(t_facility, sinks[t_logFile]);
        } catch (const spdlog::spdlog_ex &floggerExc) {
            std::cerr << "File logger init failed, defaulting to stderr: " << floggerExc.what() << std::endl;
            try {
                logger = spdlog::stderr_color_mt(t_facility);
            } catch (const spdlog::spdlog_ex &exc) {
                std::cerr << "stderr logger init failed, aborting due to fatal error " << exc.what() << std::endl;
            }
        }

        return logger;
    }

    void LogHandler::logSeparator(const std::string &t_logFile) {
        // Create temporary logger instance.
        std::shared_ptr<spdlog::logger> _ = createLogger("LOG_SEPARATOR", t_logFile);

        // Set the pattern to be a static dotted line.
        _->set_pattern("\n" + std::string(80, '-'));

        // Print the static pattern.
        _->info("");

        // Flush the output to file.
        _->flush();

        // Reset the pattern (NB: set_pattern changes for the entire sink, not just logger!).
        resetPattern(_);

        // Remove the temporary logger instance from the registry.
        spdlog::drop("LOG_SEPARATOR");
    }

    void LogHandler::resetPattern(std::shared_ptr<spdlog::logger> &t_logger) {
        t_logger->set_pattern(defaultLogPattern);
    }

    void LogHandler::resetPattern(std::shared_ptr<spdlog::sinks::basic_file_sink_mt> &t_sink) {
        t_sink->set_pattern(defaultLogPattern);
    }
} // namespace sane