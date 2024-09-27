    .data
str_intr_num: 
     .asciiz "Introduzca un número primo: "
str_es_primo:
     .asciiz "Correcto. El número es primo. \n"  
iterador:
     .word 2


     .text
#int main ()
main:

#std::cout << "Introduzca un número primo" << std::endl;
li, $v0, 4 #Imprime ascii
la, $a0, str_intr_num #Carga la dirección en $a0
syscall

lw, $s2, iterador #iterador i => $s2

#std::cin >> x;
li $v0, 5 #Pide un entero al usuario
syscall #Llamada al sistema

move $s0, $v0 # $s0 = $v0

#Bucle que comprueba que el número sea primo
loop:

#x % i != 0
div $s0, $s2 #Divide $s0/$s2, almacena el cociente en LO y el cociente en HI
mfhi $s3 #Guarda el resto desde HI al registro $s3

beq $s3, $zero, main #Salto si el tresto es igual a 0
add $s2, $s2, 1 #Le añadimos 1 al iterador i, tiene valor 1, por lo que comenzaría con valor 2
blt $s2, $s0, loop #Cantidad de veces que se repite el bucle, mientras que $s2 < $s0

j res_primo #Si no se encuentran valores que den 0 y se acaba el bucle, salto incondicional a la etiqueta "res_primo"

#std::cout << "Correcto. El número es primo." << std::endl;
res_primo:
li, $v0, 4 #Imprime ascii
la, $a0, str_es_primo #Carga la dirección en $a
syscall

#Finalización del programa
li, $v0, 10
syscall