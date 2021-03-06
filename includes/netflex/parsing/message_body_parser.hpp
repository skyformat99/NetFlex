// The MIT License (MIT)
//
// Copyright (c) 2015-2017 Simon Ninon <simon.ninon@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <list>
#include <memory>

#include <netflex/parsing/parser_iface.hpp>

namespace netflex {

namespace parsing {

class message_body_parser : public parser_iface {
public:
  //! ctor & dtor
  message_body_parser(http::request& request);
  ~message_body_parser(void) = default;

  //! copy ctor & assignment operator
  message_body_parser(const message_body_parser&) = delete;
  message_body_parser& operator=(const message_body_parser&) = delete;

public:
  //! parser_iface impl
  parser_iface& operator<<(std::string&);
  bool is_done(void) const;

private:
  //! parse body by delegating to other appropriate parsers
  bool parse_body(std::string&);

private:
  //! parsing state
  enum class state {
    content_length,
    chuncked,
    compress,
    deflate,
    gzip,
    done
  };

  //! build states list from request headers
  std::list<state> build_states_from_request_headers(void) const;

  //! create parser from given state
  std::unique_ptr<parser_iface> create_parser_from_state(state s) const;

private:
  //! list of states to process, current state is head of list
  std::list<state> m_states;
  //! current parser
  std::unique_ptr<parser_iface> m_current_parser;
};

} // namespace parsing

} // namespace netflex
