#ifndef LOG_PARSER_H
#define LOG_PARSER_H

#include <cpplog/log.hpp>

namespace logging {
    struct Log_Record{
        struct Log_Time {
            unsigned int second;
            unsigned int minute;
            unsigned int hour;
        };
        Log_Time time;

        struct Log_Date {
            unsigned int day;
            unsigned int month;
            unsigned int year;
        };
        Log_Date date;

        struct Log_File_Info {
            std::string filename;
            std::string function_name;
            unsigned int line_number;
        };
        Log_File_Info file_info;

        Severity severity;
        std::string message;
    };
    
    Log_Record parse_log_message_to_record(const std::string_view& log_msg);

    struct Log_Record_Parse_Exception : std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    std::ostream& operator<<(std::ostream& out, const Log_Record& record);
}

#endif
