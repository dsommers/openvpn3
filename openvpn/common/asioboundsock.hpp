//    OpenVPN -- An application to securely tunnel IP networks
//               over a single port, with support for SSL/TLS-based
//               session authentication and key exchange,
//               packet encryption, packet authentication, and
//               packet compression.
//
//    Copyright (C) 2012-2016 OpenVPN Technologies, Inc.
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License Version 3
//    as published by the Free Software Foundation.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program in the COPYING file.
//    If not, see <http://www.gnu.org/licenses/>.

// Asio TCP socket that can be configured so that open() method
// always prebinds the socket to a given local address.  Useful
// for TCP clients.

#ifndef OPENVPN_COMMON_ASIOBOUNDSOCK_H
#define OPENVPN_COMMON_ASIOBOUNDSOCK_H

#include <asio.hpp>

#include <openvpn/addr/ip.hpp>
#include <openvpn/common/extern.hpp>

namespace openvpn {
  namespace AsioBoundSocket {

    typedef asio::basic_stream_socket<asio::ip::tcp> SocketBase;

    class Socket : public SocketBase
    {
    public:
      explicit Socket(asio::io_context& io_context)
	: SocketBase(io_context)
      {
      }

      void bind_local(const IP::Addr& addr)
      {
	bind_local_addr = addr;
      }

    private:
      virtual void async_connect_post_open(const protocol_type& protocol, asio::error_code& ec) override
      {
	if (bind_local_addr.defined())
	  {
	    set_option(asio::socket_base::reuse_address(true), ec);
	    if (ec)
	      return;
	    bind(asio::ip::tcp::endpoint(bind_local_addr.to_asio(), 0), ec); // port 0 -- kernel will choose port
	  }
      }

      IP::Addr bind_local_addr;
    };

  }
}

#endif
