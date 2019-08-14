#ifndef SANE_TYPES_HPP
#define SANE_TYPES_HPP

#include <string>
#include <ctime>

#include <time.h>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <iostream>
namespace sane {
    extern "C" char* strptime(const char* s, const char* f, struct tm* tm);

//    char *strptime(const char * __restrict, const char * __restrict, struct tm * __restrict);

    struct datetime_t {
        bool isEmpty = true;

        // YYYY-MM-DDThh:mm:ss.sssZ
        std::string iso8601;

        // YYYY-MM-DD HH:MM:SS
        std::string isoDateAndTime;

        // UNIX Timestamp
        unsigned int timestamp;

        // As individual Segments (Using std::tm struct as template)
        int year;           // Years since 1900
        int month;          // Months since January     [0,  11]
        int day;            // Day of the month         [1,  31]
        int weekDay;        // Days since Sunday        [0,   6]
        int yearDay;        // Days since January 1st   [0, 365]

        int hour;           // Hours since midnight     [0,  23]
        int minute;         // Minutes after the hour   [0,  59]
        int second;         // Seconds after the minute [0,  60]
        int millisecond;    // Milliseconds [000-999]

        // Pesky, pesky DST ~
        int isDST;          // Daylight Saving Time flag.
                            // The value is positive if DST is in effect,
                            // zero if not and negative if no information is available.

        // Format strings
        const char* ISO8601_FORMAT = "%Y-%m-%dT%H:%M:%S."; // millisecond not in tm struct spec

        void fromISO8601(const std::string &t_iso8601) {
            iso8601 = t_iso8601;

            // Strip out milliseconds to make string tm struct compatible.
            const std::string iso8601Substr = t_iso8601.substr(0, t_iso8601.size() -5);

            // Create an std::tm struct and strptime the ISO8601 string.
            tm timeInfo = tm();
            strptime(t_iso8601.c_str(), ISO8601_FORMAT, &timeInfo);

            // Calculate UNIX timestamp
            timestamp = timegm(&timeInfo);

            // Set humanized date & time.
            isoDateAndTime = iso8601Substr;
            std::replace(isoDateAndTime.begin(), isoDateAndTime.end(), 'T', ' ');

            // Assign individual values
            year    = timeInfo.tm_year;
            month   = timeInfo.tm_mon;
            day     = timeInfo.tm_mday;
            weekDay = timeInfo.tm_wday;
            yearDay = timeInfo.tm_yday;
            hour    = timeInfo.tm_hour;
            minute  = timeInfo.tm_min;
            second  = timeInfo.tm_sec;
            try {
                std::string stringToConvert = t_iso8601.substr(t_iso8601.size() -4, 3);
                millisecond = std::stoi(stringToConvert);
            } catch(std::invalid_argument& e){
                // if no conversion could be performed
                std::cerr << "datetime_t.fromISO8601(" << t_iso8601 << "): " << "Invalid argument exception!"
                          << std::endl;
            } catch(std::out_of_range& e){
                // if the converted value would fall out of the range of the result type
                // or if the underlying function (std::strtol or std::strtoull) sets errno
                // to ERANGE.
                std::cerr << "datetime_t.fromISO8601(" << t_iso8601 << "): " <<"Out of range exception" << std::endl;
            } catch (const std::exception &exc) {
                std::cerr << "datetime_t.fromISO8601(" << t_iso8601 << "): " << "Unexpected exception: "
                          << std::string(exc.what()) << "!" << std::endl;
            }
            isDST = timeInfo.tm_isdst;

            isEmpty = false;
        }

        bool empty() {
            return isEmpty;
        }
    };
} // namespace sane
#endif //SANE_TYPES_HPP

