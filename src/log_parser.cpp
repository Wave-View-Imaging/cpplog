#include <iostream>

#include <cpplog/log_parser.hpp>

namespace logging {

    Log_Record parse_log_message_to_record(const std::string_view& log_msg) {
        const auto parse_string = [&](char to_find, size_t start_pos) {
            const auto pos_of_find = log_msg.find_first_of(to_find, start_pos);
            const auto str = log_msg.substr(start_pos, pos_of_find - start_pos);
            return std::make_pair(pos_of_find, str);
        };

        const auto parse_int = [&](char to_find, size_t start_pos) {
            const auto pos_and_str = parse_string(to_find, start_pos);
            return std::make_pair(
                pos_and_str.first, 
                static_cast<unsigned int>(std::stoi(pos_and_str.second.data()))
            );
        };

        const auto to_upper = [](std::string&& str) {
            std::string s = std::move(str);
            std::transform(s.begin(), s.end(), s.begin(), 
                    [](unsigned char c){ return std::toupper(c); }
                    );
            return s;
        };

        const auto parse_severity = [&](std::string&& severity_str) {
            std::string upper_str = to_upper(std::move(severity_str));
            if (upper_str == "TRACE") return Severity::Trace;
            if (upper_str == "DEBUG") return Severity::Debug;
            if (upper_str == "INFO") return Severity::Info;
            if (upper_str == "WARN") return Severity::Warn;
            if (upper_str == "ERROR") return Severity::Error;
            if (upper_str == "FATAL") return Severity::Fatal;

            std::ostringstream error_str_stream;
            error_str_stream << "Unable to parse \"" << severity_str << "\" to type Severity";
            throw Log_Record_Parse_Exception { error_str_stream.str() };
        };

        const auto year_pos_int = parse_int('-', 0);
        const auto month_pos_int = parse_int('-', year_pos_int.first + 1);
        const auto day_pos_int = parse_int(' ', month_pos_int.first + 1);
        const Log_Record::Log_Date log_date = {day_pos_int.second, month_pos_int.second, year_pos_int.second};

        const auto hr_pos_int = parse_int(':', day_pos_int.first + 1);
        const auto min_pos_int = parse_int(':', hr_pos_int.first + 1);
        const auto s_pos_int = parse_int(' ', min_pos_int.first + 1);
        const Log_Record::Log_Time log_time = {s_pos_int.second, min_pos_int.second, hr_pos_int.second};

        const auto filename_and_pos = parse_string(':', s_pos_int.first + 1);
        const auto func_name_and_pos = parse_string(':', filename_and_pos.first + 1);
        const auto line_num_and_pos = parse_int(' ', func_name_and_pos.first + 1);
        const Log_Record::Log_File_Info log_file_info = {
            std::string{filename_and_pos.second},
            std::string{func_name_and_pos.second},
            line_num_and_pos.second
        };
        
        const auto severity_str_and_pos = parse_string(':', line_num_and_pos.first + 1);
        const auto severity = parse_severity(std::string(severity_str_and_pos.second));

        const auto msg_and_pos = parse_string('\n', severity_str_and_pos.first + 2);
        const auto msg = std::string{msg_and_pos.second};

        return Log_Record {
            log_time,
                log_date,
                log_file_info,
                severity,
                msg
        };
    }

    std::ostream& operator<<(std::ostream& out, const Log_Record& record) {
        const auto print_spaces = [](unsigned int num_spaces) {
            for (auto i = 0; i < num_spaces; ++i) std::cout << " ";
        };
        const auto indent_spaces  = 4;

        out << "{\n";

        print_spaces(indent_spaces);
        out << "Time {\n";
        print_spaces(2*indent_spaces);
        out << "Hour: " << record.time.hour << "\n";
        print_spaces(2*indent_spaces);
        out << "Minute: " << record.time.minute << "\n";
        print_spaces(2*indent_spaces);
        out << "Second: " << record.time.second << "\n";
        print_spaces(indent_spaces);
        out << "}\n";

        print_spaces(indent_spaces);
        out << "Log_Date {\n";
        print_spaces(2*indent_spaces);
        out << "Year: " << record.date.year << "\n";
        print_spaces(2*indent_spaces);
        out << "Month: " << record.date.month << "\n";
        print_spaces(2*indent_spaces);
        out << "Day: " << record.date.day << "\n";
        print_spaces(indent_spaces);
        out << "}\n";

        print_spaces(indent_spaces);
        out << "File Info {\n";
        print_spaces(2*indent_spaces);
        out << "Filename: " << record.file_info.filename << "\n";
        print_spaces(2*indent_spaces);
        out << "Function: " << record.file_info.function_name << "\n";
        print_spaces(2*indent_spaces);
        out << "Line Number: " << record.file_info.line_number << "\n";
        print_spaces(indent_spaces);
        out << "}\n";

        print_spaces(indent_spaces);
        out << "Severity: " << record.severity << "\n";

        print_spaces(indent_spaces);
        out << "Message: " << record.message;

        out << "\n}\n";

        return out;
    }
}
