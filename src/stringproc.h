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
#ifndef STRINGPROC_H
#define STRINGPROC_H

namespace stringproc
{
    constexpr int EXIT_ERR_MEM_ALLOC = 2;

    class ClArgs
    {
      private:
        int arg_count = 0;
        char** arg_values = nullptr;

      public:
        ClArgs(int argc, char** argv);
        virtual ~ClArgs() noexcept;
        virtual bool match(const char* arg_spec) const;

        // @throws ArgsException
        virtual void last_arg(int idx) const;

        // @throws ArgsException
        virtual const char* get_arg(int idx) const;
    };

    class ArgsException : public std::exception
    {
      public:
        virtual ~ArgsException() noexcept;
    };
}

#endif // STRINGPROC_H
