#include <stdio.h>
#include <stdlib.h>

#include "tablaSimbolos.h"
#include "analizadorSintactico.h"
#include "errores.h"
#include "lex.yy.h"


//fichero con el código fuente
FILE *fuente;

int main(int argc, char const *argv[])
{

    //INICIALIZAR ESTRUCTURAS DE DATOS
    //abrir fichero en modo lectura
    fuente = fopen("concurrentSum.go", "r");

    if(fuente == NULL){
        //error si no se puede acceder al archivo
        mostrarError(2);
        exit(1);
    }

    inicializarTabla();
    imprimirTabla();
    
    abrirFichero(fuente);


    //COMENZAR BUCLE
    //llamar a la función del analizador sintáctico.
    comenzarLectura();

    //LIBERAR ESTRUCTURAS DE DATOS
    imprimirTabla();
    //cerrar el fichero
    fclose(fuente);
    //destruir la tabla de símbolos
    destruirTabla();
    
    
    return 0;
}
