//
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <array>

int main () {
    constexpr size_t tam_buffer{256};
    int pipefd[2];
    int pipe_result = pipe(pipefd);
    if (pipe_result < 0) {
        return errno;
    }
    pid_t pid = fork();
    if (pid == 0) {
        // Proceso hijo porque pid == 0
        close(pipefd[0]); // ciero la lectura
        // En el proj
        // redirección salida estandar al extremo de escritura con dup2
        int dup2_res = dup2(pipefd[1], 1);
        if (dup2_res < 0) {
            int error = errno;
            close(pipefd[1]);
            return errno;
        }
        // Ejecutar programa con exec
        std::string say{"HOlA"};
        std::cout << say;
        close (pipefd[1]);
        return EXIT_SUCCESS;
        /*
        ssize_t nbytes=write(pipefd[1], say.data(), say.size()); // Escribo en la tubería
        if (nbytes < 0) {
          int error = errno;
          close(pipefd[1]);
          return errno;
        } else {
            close(pipefd[1]);
            return EXIT_SUCCESS;
        }*/

    } else if (pid > 0) {
      int status{};
      close(pipefd[1]);
      int result_waitpid=waitpid(pid, &status, 0);
      if (result_waitpid < 0) {
          int error = errno;
          close (pipefd[0]);
          return error;
      }
      if (WIFEXITED(status)) {
        if (WEXITSTATUS(status) == EXIT_SUCCESS) {
          // Leer contenido de la tubería
          std::array <char, tam_buffer> buffer{};
          bool flag{true};
          std::string listen{};
          while(flag) {
            ssize_t nbytes = read(pipefd[0], buffer.data(), tam_buffer);
            if (nbytes < 0) {
                int error = errno;
                close(pipefd[0]);
                return error;
            } else {
              listen.append(buffer.data(), 0, static_cast<size_t>(nbytes));
              if (nbytes < tam_buffer) {
                flag = false; // terminamos con el buffer

              }
            }
            close (pipefd[0]);
            std::cout << "Proceso padre recibe:\n" << listen << std::endl;
            return EXIT_SUCCESS;
          }
        } else {
            close (pipefd[0]);
            std::cerr << "Proceso hijo terminó con error\n";
            return WEXITSTATUS(status);
        }
      } else {
        int error = errno;
        close (pipefd[0]);
        return error;
      }
    } else {
        // Error en pid
        int error = errno;
        close(pipefd[0]); // Usar safefd
        close(pipefd[1]); //
        return errno; 
    }
   return EXIT_SUCCESS;
}