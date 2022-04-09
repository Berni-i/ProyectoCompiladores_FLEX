#include <stdio.h>
#include "definiciones.h"


//función para pasar como argumento el fichero que contendrá el código que se quiere analizar
void abrirFichero(FILE* fichero);

//función que permitirá ir obteniendo un puntero a la estructura que contenga el siguiente
//componente léxico o el valor NULL en caso de que se haya alcanzado EOF
tipoelem* siguienteElemento();

//función que permitirá llamar a yylex_destroy, que liberará los recursos utilizado por el escáner
void terminarAnalisis();

