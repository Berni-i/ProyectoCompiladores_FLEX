#include <stdio.h>
#include <stdlib.h>
#include "definiciones.h"

void mostrarError(int codigo){
    printf("ERROR: ");
    
    switch (codigo)
    {
    case 1:
        //en esta práctica no saltará nunca este error
        printf("Se ha sobrepasado el tamaño máximo de lexema. Tamaño máximo: %d\n", TAMCADENA);
        break;

    case 2:
        printf("No se puede acceder al archivo con el código fuente\n");
        break;
    
    case 3:
        printf("Número sin exponente\n");
        break;

    case 4:
        printf("Prefijo hexadecimal sin número\n");
        break;

    case 5:
        printf("Prefijo binario sin número\n");
        break;

    case 6:
        printf("Prefijo octal sin número\n");
        break;

    }
}