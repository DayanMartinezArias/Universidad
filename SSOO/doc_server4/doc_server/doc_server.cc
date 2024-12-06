#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <expected>
#include <fcntl.h>     
#include <sys/mman.h> 
#include <format>
#include <sstream>
#include <charconv>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <array>
#include <filesystem>

#include "SafeFd.h"
#include "SafeMap.h"
#include "socket_file.h"

constexpr size_t tam_buffer{50000};

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
            if (port < 0 || port > 65535) {
              return std::unexpected(parse_args_errors::missing_argument);
            }
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
      if (port < 0 || port > 65535) {
        return std::unexpected(parse_args_errors::missing_argument);
      }
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

  if (path.find("/..") != std::string::npos) {
    return std::unexpected("ESCAPE");
  }
  if (path.back() == '/') {
    std::cout << "error aqui" << std::endl;
    return std::unexpected("INCOMPLETE");
  }
  if (path.length() >= 4 && path.substr(path.length() - 4, 4) == "/bin") { // For some reason, when you use 'path/bin' without a file or any other directory after bin it gets stuck
    std::cout << "error aqui2" << std::endl;
    return std::unexpected("INCOMPLETE");
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

  std::string line_break{"\n"};
  ssize_t bytes_sent_ln = send(new_fd.get(), line_break.data(), line_break.size(), 0);
  if (bytes_sent_ln < 0) {
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

struct execute_program_error {
  int exit_code;
  int error_code;
};

struct exec_environment {
  std::string REQUEST_PATH; // ruta
  std::string SERVER_BASEDIR; // directorio base
  std::string REMOTE_PORT; // puerto remoto
  std::string REMOTE_IP;  // ip remota
};

std::expected<std::string, execute_program_error>
execute_program(const std::string& path, const exec_environment& env) {
  exec_environment aux = env; //
  execute_program_error error;

  const char *c_path = path.c_str();
  SafeFD valid(access(c_path, (F_OK && X_OK)));
  if (valid.get() < 0) {
    error.error_code = errno = errno;
    std::cerr << "failed: " << std::strerror(errno) << std::endl;
    return std::unexpected(error);
  }

  std::cout << "creating pipe" << std::endl;
  int pipefd[2];
  int result = pipe(pipefd);
  if (result < 0) {
    error.error_code = errno;
    return std::unexpected(error);
  }

  SafeFD rd(pipefd[0]);
  SafeFD wr(pipefd[1]);
 
  std::cout << "creating child process" << std::endl;
  pid_t pid = fork();
  if (pid == 0) {
    // Proceso hijo porque pid == 0
    rd = SafeFD(); // ciero la lectura
    // En el prog
    // redirección salida estandar al extremo de escritura con dup2
    std::cout << "redirecting standar output to write end" << std::endl;
    int dup2_res = dup2(wr.get(), 1);
    std::cout << "redirecting standar output to write end" << std::endl;
    if (dup2_res < 0) {
      wr = SafeFD();
      error.error_code = errno;
      error.exit_code = 1;
      return std::unexpected(error);
    }
    
    std::cout << "executing program : " << path << std::endl;
    if (execl(path.c_str(), path.c_str(), (char*)NULL) == -1) {
      error.error_code = errno;
      error.exit_code = 1;
      return std::unexpected(error);
    } 
  } else if (pid > 0) {
    int status{};
    wr = SafeFD();
    int result_waitpid=waitpid(pid, &status, 0);
    if (result_waitpid < 0) {
      std::cerr << "wait for child process failed" << std::strerror(errno) << std::endl;
      error.error_code = errno;
      rd = SafeFD();
      return std::unexpected(error);   
    } std::cout << "father is checking for exit status of child process" << std::endl;
    if (WIFEXITED(status)) {
      if (WEXITSTATUS(status) == EXIT_SUCCESS) {
      std::cout << "child process ended ok" << std::endl;
      // Leer contenido de la tubería
        std::array <char, tam_buffer> buffer{};
        bool flag{true};
        std::string listen{};
        while(flag) {
          std::cout << "father process is receiving data" << std::endl;
          ssize_t nbytes = read(rd.get(), buffer.data(), tam_buffer);
          if (nbytes < 0) {
            std::cerr << "pipe reading failed: " << std::strerror(errno) << std::endl;
            error.error_code = errno;
            rd = SafeFD();
            return std::unexpected(error); 
          } else {
            listen.append(buffer.data(), 0, static_cast<size_t>(nbytes));
            if (static_cast<size_t>(nbytes) < tam_buffer) {
              flag = false; // terminamos con el buffer
            }
          }
          return listen;
        }
      } else {
         error.exit_code = WEXITSTATUS(status);
         error.error_code = 0;
         return std::unexpected(error);  // El hijo terminó con error
        }
    } else {
      error.error_code = errno;
      std::cerr << "child process failed: " << std::strerror(errno) << std::endl;
      return std::unexpected(error);  // Hubo un problema con el estado de salida del hijo
    }
  } else {
    // Error en fork
    error.error_code = errno;
    std::cerr << "child process failed: " << std::strerror(errno) << std::endl;
    return std::unexpected(error);
  }
  return std::unexpected(error);
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
    std::cout << "Bsse directory selcted: " << options.value().directory << std::endl;
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
      if (relative_path.error() == "PROCESS KILLED") {
        std::cerr << "Process was killed before a request" << std::endl;
      } else if (relative_path.error() == "ESCAPE") {
        std::cerr << "User tried to escape from base directory" << std::endl;
      } else if (relative_path.error() == "INCOMPLETE") {
        std::cerr << "Incomplete route" << std::endl;
      } else {
        std::cerr << "User requested: " << relative_path.error() << std::endl;
      }
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
    // si no existe
    std::string absol_path = base + relative_path.value();
    std::string sub{"//"};
    while (absol_path.find(sub) != std::string::npos) {
      absol_path.replace(absol_path.find(sub), 2, "/");
    }

    if (options.value().verbose) {
      std::cout << "Client requested file " << absol_path << std::endl;
    }

    if (absol_path.find("/bin") != std::string::npos) {
      exec_environment env = SetEnvir();
      auto std_output = execute_program(absol_path, env);
      if (!std_output.has_value()) {
        if (options.value().verbose) {
          std::cout << "exited with exist status: " << std_output.error().exit_code << std::endl;
          std::cout << "exited with error code: " << std_output.error().error_code << std::endl;
          std::cout << "-----------------------------" << std::endl;
        }
        if (send_response(connection_accept.value(), "500 Internal Server Error\n") == ECONNRESET) {
          std::cerr << "Could not send a response" << std::endl;
          continue;
        }
        continue;
      } else {
        size_t sz{std_output.value().size()};
        header = std::format("Content-Length: {}\n", sz);
        send_response(connection_accept.value(), header, std_output.value());
        continue;
      }
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
