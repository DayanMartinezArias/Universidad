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
#include <charconv>
#include <cstring>

#include "SafeFd.h"
#include "SafeMap.h"
#include "socket_file.h"

std::string GetEnv(const std::string& env_var);
std::string getcwd();

enum class parse_args_errors {
  missing_argument,
  unknown_option,
  missing_file_name,
};

struct program_options {
  bool show_help = false;
  bool verbose = false;
  uint16_t port;
  std::string directory = GetEnv("DOCSERVER_BASEDIR");
  
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
          int port;
          auto [ptr, ec] = std::from_chars(it->data(), it->data() + it->size(), port);
          if (ec == std::errc()) {
            options.port = static_cast<uint16_t>(port);
          } else {
            return std::unexpected(parse_args_errors::missing_argument);
          }
        } else {
           return std::unexpected(parse_args_errors::missing_argument);
        }
    } else if (*it == "-b" || *it == "--base") {
       if (++it != end) {
         options.directory = *it;
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

  if (options.directory.empty()) {
    options.directory = getcwd();
  }
  
  std::string port_env{"DOCSERVER_PORT"};
  std::string port_str = GetEnv(port_env);

 if (options.port == 0 && !port_str.empty()) {
   int port;
    auto [ptr, ec] = std::from_chars(port_str.data(), port_str.data() + port_str.size(), port);
    if (ec == std::errc()) {
      options.port = static_cast<uint16_t>(port);
    }
  } else if (options.port == 0 && port_str.empty()) {
    options.port = 8080;
  }
  return options;
}

std::expected<std::string, int> receive_request(const SafeFD& socket, size_t max_size) {
  std::string buffer(4096, '0');
  std::cout << "Receiving data from user" << std::endl;
  ssize_t rec = recv(socket.get(), buffer.data(), max_size, 0);
  if (rec < 0) {
    return std::unexpected(errno);
  }
  size_t new_sz = static_cast<size_t>(rec);
  buffer.resize(new_sz);
  return buffer;
}

std::expected<std::string, std::string> Process(const std::string& request) {
  if (request.empty()) {
    return std::unexpected(request);   
  }
  std::string get_req, path;
  std::istringstream iss(request);
  size_t end_of_line = request.find("\r\n");
  if (end_of_line == std::string::npos) {
    return std::unexpected("PROCESS KILLED");
  }
  std::string fl{request.substr(0, end_of_line)};
  iss >> get_req >> path;
  if (get_req != "GET") {
    return std::unexpected(request);
  }

  if (path.empty()) {
    return std::unexpected(request); 
  }

  return path;
}

std::string GetEnv(const std::string& env_var) {
  const char* env_value = getenv(env_var.c_str());
  return env_value ? env_value : "";
}

 std::string getcwd() {
   char buff[255];
   return std::string(getcwd(buff, 255));
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

  return EXIT_SUCCESS;
}

std::expected<SafeMap, int> ReadAll(const std::string& path, bool verbose) {
  // Primero se abre el archivo, con los permisos adecuados según si solo se leerá o también se escribirá.
  if (verbose) {
    std::cout << "open: file " << path << std::endl;
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
  size_t sz_t{static_cast<size_t>(sz)};

  if (sz_t == 0) {
    return SafeMap{std::string_view{}, 0};
  }

  // Se mapea el archivo completo en memoria para solo lectura y de forma privada.
  void* mem{mmap(NULL, sz_t, PROT_READ, MAP_PRIVATE, fd.get(), 0)};
  if (mem == MAP_FAILED) {
    return std::unexpected(errno);
  }

  if (verbose) {
    std::cout << "read: " << sz_t << " bytes read from file" << std::endl;
  }
  // Ahora se puede acceder a los datos del archivo como si estuvieran en la memoria.
  // Por ejemplo, imprimir os primeros 10 caracteres por la consola.
   return SafeMap{std::string_view(static_cast<char*>(mem), sz_t), sz_t}; // Retornar vista del archivo
}

void help() {
  std::cout << "Use: server [options]" << std::endl;
  std::cout << "-h, --help: Show a help message on how to use the program" << std::endl;
  std::cout << "-v, --verbose: Shows informative messages through the terminal" << std::endl;
  std::cout << "-p, --port: Selects the port in wich the socket will be listening on" << std::endl;
  std::cout << "-b, --base: Selects a base directory from wich clients can request" << std::endl;
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

  std::string header;

  sockaddr_in client_addr{};
  while (true) {
    auto connection_accept = accept_connection(sock.value(), client_addr, options.value().verbose);
    if (!connection_accept.has_value()) {
      std::cerr << "Error accepting connection: " << std::endl;
      return EXIT_FAILURE;
    }

    // Recivir la petición
    auto receive = receive_request(connection_accept.value(), 4096);
    if (!receive.has_value()) {
      if (receive.error() == ECONNRESET) {
        std::cerr << "Error receiving request: " << std::endl;
        continue;
      } else {
        std::cerr << "Unknown error receiving request" << std::endl;
        EXIT_FAILURE;
      }
    }

    auto relative_path = Process(receive.value());
    if (!relative_path.has_value()) {
      std::cerr << "ERROR sent: 400 BAD request" << std::endl;
      std::cerr << "User requested: " << relative_path.error() << std::endl;
      header = "400 BAD REQUEST";
      if (options.value().verbose) {
        std::cout << "send: sending response" << std::endl;
        std::cout << "-----------------------------" << std::endl;
      }
      if(send_response(connection_accept.value(), header) ==  ECONNRESET) {
        std::cerr << "Could not send a response" << std::endl;
        continue;
      }   
      continue;
    }
    
    std::string base = options.value().directory;
    std::string absol_path = base + relative_path.value();
    std::string sub{"//"};
    while (absol_path.find(sub) != std::string::npos) {
      absol_path.replace(absol_path.find(sub), 2, "/");
    }

    if (options.value().verbose) {
      std::cout << "Client requested file " << absol_path << std::endl;
    }

    auto content = ReadAll(absol_path, options.value().verbose);
    if (!content.has_value()) {
      if (content.error() == 13) {
        header = "403 FORBIDDEN";
        if (options.value().verbose) {
          std::cout << "send: sending response" << std::endl;
          std::cout << "-----------------------------" << std::endl;
        }
      }
      else if (content.error() == 2) {
        if (options.value().verbose) {
          std::cout << "send: sending response" << std::endl;
          std::cout << "-----------------------------" << std::endl;
        }
        header = "404 NOT FOUND";
      }  
      if(send_response(connection_accept.value(), header) ==  ECONNRESET) {
        std::cerr << "Could not send a response" << std::endl;
        continue;
      }
      continue;
    }
    size_t sz{content.value().get().size()};
    header = std::format("Content-Length: {}\n", sz);
    if (options.value().verbose) {
      std::cout << "send: sending response" << std::endl;
      std::cout << "-----------------------------" << std::endl;
    }
    if(send_response(connection_accept.value(), header, content.value().get()) ==  ECONNRESET) {
      std::cerr << "Could not send a response" << std::endl;
      continue;
    }
  }
  return EXIT_SUCCESS;
}
