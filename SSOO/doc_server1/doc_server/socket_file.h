#ifndef SOCKET_FILE_H
#define SOCKET_FILE_H

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <expected>
#include <fcntl.h>   
#include <unistd.h>  
#include <sys/mman.h> 
#include <format>
#include <sstream>

std::expected<SafeFD, int> make_socket(uint16_t port) {
  // Primer parametro: Familia de protocolo
  //Segundo parametro: Tipo de socket
  SafeFD sock_fd{socket(AF_INET, SOCK_STREAM, 0)};
  if (!sock_fd.is_valid()) {
    return std::unexpected(errno);
  }

  // BIND
  sockaddr_in local_address{};                            // <1>
  local_address.sin_family = AF_INET;                     // <2>
  local_address.sin_addr.s_addr = htonl(INADDR_ANY);      // <3>
  local_address.sin_port = htons(port);                   // <4>

  int result = bind(
    sock_fd.get(),                                            // <5>
    reinterpret_cast<const sockaddr*>(&local_address),  // <6>
	  sizeof(local_address)                               // <7>
  );

  if (result < 0) {
    return std::unexpected(errno);
  }
  return sock_fd;
}

int listen_connection(const SafeFD& sock_fd) {

  if (listen(sock_fd.get(), 5) < 0) {
    return errno;
  }

  return EXIT_SUCCESS;
}

std::expected<SafeFD, int> accept_connection(const SafeFD& sock_fd, sockaddr_in& client_addr) {
  socklen_t source_address_length = sizeof(client_addr);

  // Aceptar la primera conexión que llegue
  SafeFD new_fd{accept(sock_fd.get(), reinterpret_cast<sockaddr*>(&client_addr), &source_address_length)};
  if (!new_fd.is_valid()) {
    return std::unexpected(errno);
  }
  return new_fd;
}

#endif
