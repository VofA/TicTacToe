#ifndef HPP_TTT_NET
#define HPP_TTT_NET

// #include <boost/array.hpp>
#include <array>
// #include <boost/asio.hpp>
#include <asio.hpp>
#include <iostream>
#include <string>

using asio::ip::tcp;

class TTTNet {
  public:
  TTTNet() {
    _isServer = true;
    _socket   = getServerSocket();
  }
  TTTNet(const std::string &host) {
    _isServer = false;
    _socket   = getClientSocket(host);
  }
  bool isServer() {
    return _isServer;
  }

  void write(const std::string &message) {
    try {
      asio::error_code error;
      asio::write((*_socket), asio::buffer(message), error);

      if (error) {
        throw asio::system_error(error);
      }
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  std::array<char, 128> read() {
    try {
      std::array<char, 128> buf;
      asio::error_code error;

      /*size_t len = */ (*_socket).read_some(asio::buffer(buf), error);

      if (error) {
        throw asio::system_error(error);
      }

      return buf;
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  private:
  tcp::socket *getClientSocket(const std::string &host) {
    try {
      asio::io_service io_service;

      tcp::resolver resolver(io_service);
      tcp::resolver::query query(host, "daytime");
      tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

      tcp::socket *socket = new tcp::socket(io_service);

      asio::connect(*socket, endpoint_iterator);
      return socket;
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  tcp::socket *getServerSocket() {
    try {
      asio::io_service io_service;

      tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13));

      tcp::socket *socket = new tcp::socket(io_service);

      acceptor.accept(*socket);

      return socket;
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  bool _isServer;
  tcp::socket *_socket;
};

#endif