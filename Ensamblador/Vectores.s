.data
ask_for_number: .asciiz "Introduce a new number to the vector: "
space: .asciiz " "
vect1: .space 800       # Vector para almacenar hasta 100 doubles (8 bytes c/u)

.text
  la $s0, vect1        # Cargar la dirección de vect1 en $s0 (inicio del vector)
  li $s1, 0            # Contador de elementos (inicialmente 0)
  li $s2, 100          # Máximo número de elementos (100)

main:
  while_read:
    beq $s1, $s2, end_while   # Si ya hemos leído 100 elementos, termina

    li $v0, 4                 # Llamada a sistema para imprimir cadena
    la $a0, ask_for_number
    syscall

    li $v0, 7                 # Leer double del usuario
    syscall

    # Comprobamos si el valor introducido es 0.0
    mov.d $f2, $f0            # Copiar $f0 a $f2 para la comparación
    li.d $f4, 0.0             # Cargar 0.0 en $f4
    c.eq.d $f2, $f4           # Comparar si $f2 es igual a 0.0
    bc1t end_while            # Si es 0.0, termina el ciclo

    store_val:
      s.d $f0, 0($s0)         # Almacenar el valor double en el vector
      addi $s0, $s0, 8        # Incrementar la dirección para el siguiente double

      addi $s1, $s1, 1        # Incrementar el contador de elementos

      j while_read            # Repetir el ciclo

end_while:

menu:
  for_write:
    la $s0, vect1             # Reiniciar el puntero del vector
    li $s3, 0                 # Inicializar contador para escritura

    beq $s3, $s1, end_for     # Si se han impreso todos los elementos, salir

    l.d $f0, 0($s0)           # Cargar el double desde el vector
    mov.d $f12, $f0           # Pasar a $f12 para la impresión

    li $v0, 3                 # Llamada a sistema para imprimir double
    syscall

    li $v0, 4                 # Llamada a sistema para imprimir espacio
    la $a0, space
    syscall

    addi $s0, $s0, 8          # Avanzar al siguiente double
    addi $s3, $s3, 1          # Incrementar el contador

    j for_write               # Repetir el ciclo de impresión

end_for:

  # Terminar el programa
  li $v0, 10                  # Código de llamada a sistema para salir
  syscall



    
    
    
    



