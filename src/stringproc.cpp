/**
 * stringproc utility
 *
 * Copyright (C) 2020 Robert ALTNOEDER
 *
 * Redistribution and use in source and binary forms,
 * with or without modification, are permitted provided that
 * the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <stdexcept>

#include <dsaext.h>
#include <integerparse.h>

#include "stringproc.h"

namespace stringproc
{
    const char* executable_name = "stringproc";

    static void length(const char* text);
    static void prepend(const char* prefix, const char* text);
    static void append(const char* suffix, const char* text);
    static void index(const char* part, const char* text);
    static void last_index(const char* part, const char* text);
    // @throws ArgsException
    static void first_part(const char* length_spec, const char* text);
    // @throws ArgsException
    static void last_part(const char* length_spec, const char* text);
    // @throws ArgsException
    static void char_at(const char* offset_spec, const char* text);
    // @throws ArgsException
    static void substring(const char* start_spec, const char* end_spec, const char* text);
    static void first_token(const char* text, const char* separator);
    static void last_token(const char* text, const char* separator);
    static void remove_first_token(const char* text, const char* separator);
    static void remove_last_token(const char* text, const char* separator);
    static void tokenize(const char* text, const char* separator);
    static void trim_leading(const char* text);
    static void trim_trailing(const char* text);
    static void trim_all(const char* text);

    static void trim_trailing_impl(std::string& text_str);
    static void trim_leading_impl(std::string& text_str);

    // @throws ArgsException
    static size_t to_size(const char *size_spec);

    static void display_syntax();
}

int main(int argc, char* argv[])
{
    int rc = EXIT_FAILURE;
    try
    {
        if (argv == nullptr)
        {
            throw stringproc::ArgsException();
        }

        if (argc >= 1)
        {
            stringproc::executable_name = argv[0];
        }

        stringproc::ClArgs arguments(argc, argv);

        if (argc < 2)
        {
            throw stringproc::ArgsException();
        }

        if (arguments.match("length of ?"))
        {
            stringproc::length(arguments.get_arg(3));
        }
        else
        if (arguments.match("prepend ? to ?"))
        {
            stringproc::prepend(arguments.get_arg(2), arguments.get_arg(4));
        }
        else
        if (arguments.match("append ? to ?"))
        {
            stringproc::append(arguments.get_arg(2), arguments.get_arg(4));
        }
        else
        if (arguments.match("index of ? in ?"))
        {
            stringproc::index(arguments.get_arg(3), arguments.get_arg(5));
        }
        else
        if (arguments.match("last index of ? in ?"))
        {
            stringproc::last_index(arguments.get_arg(4), arguments.get_arg(6));
        }
        else
        if (arguments.match("char at ? in ?"))
        {
            stringproc::char_at(arguments.get_arg(3), arguments.get_arg(5));
        }
        else
        if (arguments.match("substring ? to ? of ?"))
        {
            stringproc::substring(arguments.get_arg(2), arguments.get_arg(4), arguments.get_arg(6));
        }
        else
        if (arguments.match("first token of ? split by ?"))
        {
            stringproc::first_token(arguments.get_arg(4), arguments.get_arg(7));
        }
        else
        if (arguments.match("last token of ? split by ?"))
        {
            stringproc::last_token(arguments.get_arg(4), arguments.get_arg(7));
        }
        else
        if (arguments.match("remove first token of ? split by ?"))
        {
            stringproc::remove_first_token(arguments.get_arg(5), arguments.get_arg(8));
        }
        else
        if (arguments.match("remove last token of ? split by ?"))
        {
            stringproc::remove_last_token(arguments.get_arg(5), arguments.get_arg(8));
        }
        else
        if (arguments.match("first ? bytes of ?"))
        {
            stringproc::first_part(arguments.get_arg(2), arguments.get_arg(5));
        }
        else
        if (arguments.match("last ? bytes of ?"))
        {
            stringproc::last_part(arguments.get_arg(2), arguments.get_arg(5));
        }
        else
        if (arguments.match("tokenize ? split by ?"))
        {
            stringproc::tokenize(arguments.get_arg(2), arguments.get_arg(5));
        }
        else
        if (arguments.match("trim whitespace in ?"))
        {
            stringproc::trim_all(arguments.get_arg(4));
        }
        else
        if (arguments.match("trim leading whitespace in ?"))
        {
            stringproc::trim_leading(arguments.get_arg(5));
        }
        else
        if (arguments.match("trim trailing whitespace in ?"))
        {
            stringproc::trim_trailing(arguments.get_arg(5));
        }
        else
        {
            std::cerr << stringproc::executable_name << ": Invalid command\n\n" << std::flush;
            throw stringproc::ArgsException();
        }
        rc = EXIT_SUCCESS;
    }
    catch (stringproc::ArgsException&)
    {
        stringproc::display_syntax();
    }
    catch (std::bad_alloc&)
    {
        std::cerr << stringproc::executable_name << ": Error: Out of memory" << std::endl;
        rc = stringproc::EXIT_ERR_MEM_ALLOC;
    }

    std::cout << std::flush;
    std::cerr << std::flush;

    return rc;
}

namespace stringproc
{
    static void length(const char* text)
    {
        std::cout << strlen(text) << std::flush;
    }

    static void prepend(const char* prefix, const char* text)
    {
        std::cout << prefix << text << std::flush;
    }

    static void append(const char* suffix, const char* text)
    {
        std::cout << text << suffix << std::flush;
    }

    static void index(const char* part, const char* text)
    {
        std::string text_str(text);
        const size_t idx = text_str.find(part);
        if (idx != std::string::npos)
        {
            std::cout << idx << std::flush;
        }
        else
        {
            std::cout << -1 << std::flush;
        }
    }

    static void last_index(const char* part, const char* text)
    {
        std::string text_str(text);
        const size_t idx = text_str.rfind(part);
        if (idx != std::string::npos)
        {
            std::cout << idx << std::flush;
        }
        else
        {
            std::cout << -1 << std::flush;
        }
    }

    // @throws ArgsException
    static void first_part(const char* length_spec, const char* text)
    {
        const size_t split_idx = to_size(length_spec);
        std::string text_str(text);
        std::cout << text_str.substr(0, std::min(split_idx, text_str.length())) << std::flush;
    }

    // @throws ArgsException
    static void last_part(const char* length_spec, const char* text)
    {
        const size_t split_idx = to_size(length_spec);
        std::string text_str(text);
        const size_t text_length = text_str.length();
        std::cout << text_str.substr(text_length - std::min(split_idx, text_length), std::string::npos) << std::flush;
    }

    // @throws ArgsException
    static void char_at(const char* offset_spec, const char* text)
    {
        const size_t char_idx = to_size(offset_spec);
        if (char_idx < strlen(text))
        {
            std::cout << text[char_idx] << std::flush;
        }
    }

    // @throws ArgsException
    static void substring(const char* start_spec, const char* end_spec, const char* text)
    {
        std::string text_str(text);
        const size_t text_length = text_str.length();
        const size_t start_idx = std::min(to_size(start_spec), text_length);
        const bool use_max_end_idx = strcmp("end", end_spec) == 0;
        const size_t end_idx = use_max_end_idx ? text_length : std::min(to_size(end_spec), text_length);
        if (start_idx <= end_idx)
        {
            const size_t length = end_idx - start_idx;
            std::cout << text_str.substr(start_idx, length) << std::flush;
        }
    }

    static void first_token(const char* text, const char* separator)
    {
        std::string text_str(text);
        const size_t split_idx = text_str.find(separator);
        if (split_idx != std::string::npos)
        {
            std::cout << text_str.substr(0, split_idx) << std::flush;
        }
        else
        {
            std::cout << text_str << std::flush;
        }
    }

    static void last_token(const char* text, const char* separator)
    {
        std::string text_str(text);
        std::string separator_str(separator);
        const size_t split_idx = text_str.rfind(separator_str);
        if (split_idx != std::string::npos)
        {
            std::cout << text_str.substr(split_idx + separator_str.length(), std::string::npos) << std::flush;
        }
        else
        {
            std::cout << text_str << std::flush;
        }
    }

    static void remove_first_token(const char* text, const char* separator)
    {
        std::string text_str(text);
        std::string separator_str(separator);
        const size_t split_idx = text_str.find(separator);
        if (split_idx != std::string::npos)
        {
            std::cout << text_str.substr(split_idx + separator_str.length()) << std::flush;
        }
    }

    static void remove_last_token(const char* text, const char* separator)
    {
        std::string text_str(text);
        std::string separator_str(separator);
        const size_t split_idx = text_str.rfind(separator_str);
        if (split_idx != std::string::npos)
        {
            std::cout << text_str.substr(0, split_idx) << std::flush;
        }
    }

    static void tokenize(const char* text, const char* separator)
    {
        std::string text_str(text);
        std::string separator_str(separator);
        for (size_t split_idx = text_str.find(separator_str);
             split_idx != std::string::npos;
             split_idx = text_str.find(separator_str))
        {
            std::cout << text_str.substr(0, split_idx) << "\n";
            text_str = text_str.substr(split_idx + separator_str.length(), std::string::npos);
        }
        std::cout << text_str << "\n" << std::flush;
    }

    static void trim_leading(const char* text)
    {
        std::string text_str(text);
        trim_leading_impl(text_str);
        std::cout << text_str << std::flush;
    }

    static void trim_trailing(const char* text)
    {
        std::string text_str(text);
        trim_trailing_impl(text_str);
        std::cout << text_str << std::flush;
    }

    static void trim_all(const char* text)
    {
        std::string text_str(text);
        trim_leading_impl(text_str);
        trim_trailing_impl(text_str);
        std::cout << text_str << std::flush;
    }

    static void trim_leading_impl(std::string& text_str)
    {
        const char* const text = text_str.c_str();
        const size_t text_length = text_str.length();

        for (size_t idx = 0; idx < text_length; ++idx)
        {
            if (text[idx] != ' ' && text[idx] != '\t')
            {
                text_str = text_str.substr(idx);
                break;
            }
        }
    }

    static void trim_trailing_impl(std::string& text_str)
    {
        const char* const text = text_str.c_str();
        const size_t text_length = text_str.length();

        size_t idx = text_length;
        while (idx > 0)
        {
            --idx;
            if (text[idx] != ' ' && text[idx] != '\t')
            {
                text_str = text_str.substr(0, idx + 1);
                break;
            }
        }
    }

    static size_t to_size(const char *size_spec)
    {
        size_t result = 0;
        {
            uint64_t parsed_value = 0;
            try
            {
                parsed_value = dsaext::parse_unsigned_int64_c_str(size_spec, strlen(size_spec));
            }
            catch (dsaext::NumberFormatException&)
            {
                std::cerr << executable_name << "Input string \"" << size_spec <<
                    "\" cannot be parsed as a number\n" << std::flush;
                throw ArgsException();
            }
            if (parsed_value > SIZE_MAX || parsed_value >= std::string::npos)
            {
                std::cerr << executable_name << "Value " << parsed_value << " is out of range\n\n" << std::flush;
                throw ArgsException();
            }
            result = static_cast<size_t> (parsed_value);
        }
        return result;
    }

    static void display_syntax()
    {
        std::cerr << "Syntax:\n";
        std::cerr << "    " << executable_name << " length of <string>\n";
        std::cerr << "    " << executable_name << " prepend <string> to <string>\n";
        std::cerr << "    " << executable_name << " append <string> to <string>\n";
        std::cerr << "    " << executable_name << " index of <string> in <string>\n";
        std::cerr << "    " << executable_name << " last index of <string> in <string>\n";
        std::cerr << "    " << executable_name << " char at <offset> in <string>\n";
        std::cerr << "    " << executable_name << " substring <offset> to <offset> of <string>\n";
        std::cerr << "    " << executable_name << " first token of <string> split by <string>\n";
        std::cerr << "    " << executable_name << " last token of <string> split by <string>\n";
        std::cerr << "    " << executable_name << " remove first token of <string> split by <string>\n";
        std::cerr << "    " << executable_name << " remove last token of <string> split by <string>\n";
        std::cerr << "    " << executable_name << " first <count> bytes of <string>\n";
        std::cerr << "    " << executable_name << " last <count> bytes of <string>\n";
        std::cerr << "    " << executable_name << " tokenize <string> split by <string>\n";
        std::cerr << "    " << executable_name << " trim whitespace in <string>\n";
        std::cerr << "    " << executable_name << " trim leading whitespace in <string>\n";
        std::cerr << "    " << executable_name << " trim trailing whitespace in <string>\n";
        std::cerr << std::endl;
    }

    ClArgs::ClArgs(int argc, char** argv)
    {
        arg_count = argc;
        arg_values = argv;
    }

    ClArgs::~ClArgs() noexcept
    {
    }

    bool ClArgs::match(const char* const arg_spec) const
    {
        const size_t arg_spec_length = strlen(arg_spec);
        int arg_idx = 1;
        size_t spec_idx = 0;
        while (arg_idx < arg_count && spec_idx < arg_spec_length)
        {
            if (arg_spec[spec_idx] != '?')
            {
                if (arg_spec[spec_idx] != ' ')
                {
                    int cmp_idx = 0;
                    while (arg_values[arg_idx][cmp_idx] != '\0' && arg_spec[spec_idx] != '\0' &&
                           arg_spec[spec_idx] != ' ' && arg_values[arg_idx][cmp_idx] == arg_spec[spec_idx])
                    {
                        ++spec_idx;
                        ++cmp_idx;
                    }
                    if (arg_values[arg_idx][cmp_idx] == '\0' &&
                        (arg_spec[spec_idx] == '\0' || arg_spec[spec_idx] == ' '))
                    {
                        ++arg_idx;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                ++arg_idx;
            }
            ++spec_idx;
        }
        return arg_idx == arg_count && spec_idx >= arg_spec_length;
    }

    // @throws ArgsException
    const char* ClArgs::get_arg(const int idx) const
    {
        if (idx >= arg_count)
        {
            throw ArgsException();
        }
        return arg_values[idx];
    }

    // @throws ArgsException
    void ClArgs::last_arg(int idx) const
    {
        if (idx != arg_count - 1)
        {
            throw ArgsException();
        }
    }

    ArgsException::~ArgsException() noexcept
    {
    }
}
