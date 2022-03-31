#include <stdio.h>

//iniciar el buffer al principio del programa pasando el puntero al fichero
void iniciarBuffer(FILE *id_fichero);

//devolver el siguiente caracter del fichero
char siguienteCaracter();

//mover la posición de inicio a delantero
void saltarCaracter();

//retrasar la posición de delantero
void devolverCaracter();

//devolver la palabra una vez se alcance un estado final
char* devolverPalabra();
