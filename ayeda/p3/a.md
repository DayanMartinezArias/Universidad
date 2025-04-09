En el protocolo TCP los n´umeros de secuencia y confirmaci´on (ACK) se expresan en bytes,
de manera que el n´umero de secuencia es el byte dentro del flujo de bytes a enviar asociado
al primer byte del segmento, mientras que el n´umero de confirmaci´on (ACK) siempre es el
siguiente byte que espera recibir el receptor. Esto implica que la asociaci´on entre los segmentos
enviados y sus confirmaciones dependen, adem´as del n´umero de secuencia, del tama˜no del
segmento enviado. Esto se puede entender mejor observando la figura 4.4, donde se puede ver
que el paquete de confirmaci´on asociado a un determinado paquete es aquel cuyo n´umero de
confirmaci´on coincide con el n´umero de secuencia sum´andole el tama˜no del paquete. Recuerde
que TCP utiliza confirmaciones superpuestas o piggybacking, es decir, que si es posible no se
env´ıan paquetes de confirmaci´on individuales, sino que se a˜nade la confirmaci´on la cabecera de
un paquete de datos que viaja en sentido opuesto al paquete que est´a siendo confirmado. Por
ello, en la figura 4.4 se han marcado en rojo los datos de la cabecera TCP que corresponden a los
datos enviados en el sentido Cliente→Servidor y se han marcado en azul los que corresponden
a los datos enviados en el sentido Servidor→Cliente.
Cliente Servidor
seq = 1, ack = 1, len = 1460
seq = 1, ack = 1461, len = 40
seq = 1461, ack = 41, len = 1460
seq = 2921, ack = 41, len = 1460
seq = 41, ack = 4381, len = 0
Figura 4.4: Diagrama de flujo de paquetes.
Utilizando la captura realizada previamente, filtre los paquetes SYN con: tcp.flags.syn==1.
Seleccione el segundo paquete SYN y a continuaci´on pulse el bot´on derecho del rat´on y en el
men´u contextual elija la opci´on Follow TCP stream. Cierre la ventana emergente. Ahora f´ıjese
como el filtro de Wireshark ha cambiado a algo similar a tcp.stream eq 1, es decir, se ha seleccionado un flujo TCP dentro de la captura. Todos los paquetes que se muestran en el listado
de paquetes corresponden a la misma conexi´on TCP, que es la descarga del fichero realizada
previamente.
Conteste a la siguiente pregunta:
6. Ahora, elabore un diagrama de flujo de paquetes similar al de la figura 4.4 utilizando los
10 primeros paquetes despu´es de la fase de conexi´on. Relacione cada paquete enviado con
el paquete que contiene el n´umero de confirmaci´on correspondiente.