#include <stdio.h>
#include <stdlib.h>
#include "definiciones.h"

void mostrarError(int codigo){
    printf("ERROR: ");
    
    switch (codigo)
    {
    case 1:
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

    }
}