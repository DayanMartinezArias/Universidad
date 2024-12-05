int server () {
  // Primer parametro: Familia de protocolo
  //Segundo parametro: Tipo de socket
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd < 0) {
    return errno;
  }

  // BIND
  sockaddr_in local_address{};                            // <1>
  local_address.sin_family = AF_INET;                     // <2>
  local_address.sin_addr.s_addr = htonl(INADDR_ANY);      // <3>
  local_address.sin_port = htons(8080);                   // <4>

  int result = bind(
    sock_fd,                                            // <5>
    reinterpret_cast<const sockaddr*>(&local_address),  // <6>
	  sizeof(local_address)                               // <7>
  );
  if (result < 0) {
    return errno;
  }

// Listen
sockaddr_in source_adress{};
socklen_t source_address_length;

if (listen(sock_fd, 5) < 0) {
    return errno;
}

// Aceptar la primera conexión que llegue
int new_fd = accept(sock_fd, reinterpret_cast<sockaddr*>(&source_adress),
    &source_address_length);
if (new_fd < 0)
{
    return errno;
}

std::string message_text("¡Hola, mundo!");

ssize_t bytes_sent = send(new_fd, message_text.data(), message_text.size(), 0);
if (bytes_sent < 0)
{
   return errno;
}

close(new_fd);
close(sock_fd);

  return 0;
}