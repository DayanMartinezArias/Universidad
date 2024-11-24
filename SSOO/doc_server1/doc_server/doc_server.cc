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

#include "SafeFd.h"
#include "SafeMap.h"
#include "socket_file.h"

enum class parse_args_errors {
  missing_argument,
  unknown_option,
  missing_file_name,
  invalid_port
};

struct program_options {
  bool show_help = false;
  bool verbose = false;
  uint16_t port;
  std::string output_filename;
  
  std::vector<std::string> additional_args; 
};

std::expected<program_options, parse_args_errors> ParseArgs(int argc, char* argv[]) {
  std::vector<std::string_view> args(argv + 1, argv + argc);
  program_options options;

  for (auto it = args.begin(), end = args.end(); it != end; ++it) {
    if (*it == "-h" || *it == "--help") {
      options.show_help = true;
    } else if (*it == "-v" || *it == "--verbose") {
      options.verbose = true;
    } else if (*it == "-p" || *it == "--port") {
        if (++it != end) {
          for (char c : *it) {
            if (!std::isdigit(c)) {
              return std::unexpected(parse_args_errors::invalid_port);
            }
          }
          int port = std::stoi(std::string(*it));
          options.port = static_cast<uint16_t>(port);
        } else {
           return std::unexpected(parse_args_errors::missing_argument);
        }
    } else if (*it == "-o" || *it == "--output") {
        if (++it != end) {
            options.output_filename = *it;
        } else {
            return std::unexpected(parse_args_errors::missing_argument);
         }
    } else if (!it->starts_with("-")) {
         options.additional_args.push_back(std::string(*it));
     } else {
        return std::unexpected(parse_args_errors::unknown_option);
     }
 }
 if (options.additional_args.empty()) {
   return std::unexpected(parse_args_errors::missing_file_name);
 }

 std::string port_str{getenv("DOCSERVER_PORT")};
 
 if (options.port == 0 && !port_str.empty()) {
  for (char c : port_str) {
    if (!std::isdigit(c)) {
      return std::unexpected(parse_args_errors::invalid_port);
    }
  }
  int port = std::stoi(getenv("DOCSERVER_PORT"));
  options.port = static_cast<uint16_t>(port);
 } else if (options.port == 0 && port_str.empty()) {
  options.port = 8080;
 }
 
return options;
}

int send_response(const SafeFD& new_fd, std::string_view header, std::string_view body = {}) {
  ssize_t bytes_sent_header = send(new_fd.get(), header.data(), header.size(), 0);
  if (bytes_sent_header < 0) {
   return errno;
  }

  ssize_t bytes_sent_body = send(new_fd.get(), body.data(), body.size(), 0);
  if (bytes_sent_body < 0) {
   return errno;
  }

  std::string blank_line = "\n";
  ssize_t bytes_sent_blank = send(new_fd.get(), blank_line.data(), blank_line.size(), 0);
  if (bytes_sent_blank < 0) {
    return errno;
  }
  return EXIT_SUCCESS;
}

std::expected<SafeMap, int> ReadAll(const std::string& path, bool verbose) {
  // Primero se abre el archivo, con los permisos adecuados según si solo se leerá o también se escribirá.
  if (verbose) {
    std::cout << "open: file" << path << std::endl;
  }

  SafeFD fd{open(path.c_str(), O_RDONLY)};
  if (!fd.is_valid()) {
    return std::unexpected(errno);
  }
  // Para mapear una archivo completo es necesario conocer su tamaño.
  // Una forma es usar fstat() y otra es usar lseek().
  // La función lseek() sirve para mover el puntero de lectura/escritura de un archivo y retorna la posición
  // a la que se ha movido. Por tanto, si se mueve al final del archivo, se obtiene el tamaño de este.
  off_t sz{lseek( fd.get(), 0, SEEK_END )};
  size_t length{static_cast<size_t>(sz)};

  if (length == 0) {
    return SafeMap{std::string_view{}, 0};
  }

  // Se mapea el archivo completo en memoria para solo lectura y de forma privada.
  void* mem{mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd.get(), 0)};
  if (mem == MAP_FAILED) {
    return std::unexpected(errno);
  }

  if (verbose) {
    std::cout << "read: " << length << " bytes read from file" << std::endl;
  }

  // Ahora se puede acceder a los datos del archivo como si estuvieran en la memoria.
  // Por ejemplo, imprimir os primeros 10 caracteres por la consola.
   return SafeMap{std::string_view(static_cast<char*>(mem), length), length}; // Retornar vista del archivo
}

std::string getenv(const std::string& name) {
  char* value = getenv(name.c_str());
  if (value) {
    return std::string(value);
  }
  else {
    return std::string();
  }
}

void help() {
  std::cout << "Use: server [options]" << std::endl;
  std::cout << "-h, --help: Show a help message on how to use the program" << std::endl;
  std::cout << "-v, --verbose: Shows informative messages through the terminal" << std::endl;
  std::cout << "-p, --port: Selects the port in wich the socket will be listening on" << std::endl;
}

int main(int argc, char* argv[]) {
  auto options{ParseArgs(argc, argv)}; 
  if (!options.has_value()) {
  // Usar options.error() para comprobar el motivo del error...
  if (options.error() == parse_args_errors::missing_argument) {
    std::cerr << "There are missing arguments" << std::endl;
  } else if (options.error() == parse_args_errors::unknown_option) {
    std::cerr << "Unknown option" << std::endl;
  } else if (options.error() == parse_args_errors::missing_file_name) {
    std::cerr << "Missing file name" << std::endl;
  } else if (options.error() == parse_args_errors::invalid_port) {
    std::cerr << "Invalid port selection" << std::endl;
  }
  return EXIT_FAILURE; 
  }
  // Usar options.value() para acceder a las opciones...
  if (options.value().show_help) {
    help();
    return EXIT_SUCCESS;
  }

  if (options.value().verbose) {
    std::cout << "verbose: option selected" << std::endl;
  }

  auto sock = make_socket(options.value().port);
  if (!sock.has_value()) {
    std::cerr << "Failed to make socket" << std::endl;
    return EXIT_FAILURE;
  }
  if (listen_connection(sock.value()) != EXIT_SUCCESS) {
    std::cerr << "Failed to listen on socket" << std::endl;
    return EXIT_FAILURE;
  }

  if (options.value().verbose) {
    std::cout << "Listening for incoming connections on port " << options.value().port << "..." << std::endl;
  }

    // Accept connections in a loop
  sockaddr_in client_addr{};
  while (true) {
    auto connection = accept_connection(sock.value(), client_addr, options.value().verbose);
    if (!connection.has_value()) {
      std::cerr << "Error accepting connection: " << std::endl;
      return EXIT_FAILURE;
    }
    std::string header;

    std::string file_name(options.value().additional_args[0]);
    auto content = ReadAll(file_name, options.value().verbose);
    if (!content.has_value()) {
    if (content.error() == 13) {
      header = "403 FORBIDDEN";
    }
    else if (content.error() == 2) {
      header = "404 NOT FOUND";
    }
    if(send_response(connection.value(), header) ==  ECONNRESET) {
      std::cerr << "Could not send a response" << std::endl;
      continue;
    }
  }
  size_t sz{content.value().get().size()};
  header = std::format("Content-Length: {}\n", sz);
  if (options.value().verbose) {
    std::cout << "send: file content to be send" << std::endl;
    std::cout << "-----------------------------" << std::endl;
  }
  if(send_response(connection.value(), header, content.value().get()) ==  ECONNRESET) {
    std::cerr << "Could not send a response" << std::endl;
    continue;
  }
}

  return EXIT_SUCCESS;
}

// errores leves salida etándar 