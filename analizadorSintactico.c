#include <stdio.h>
#include <stdlib.h>

#include "analizadorLexico.h"
#include "string.h"


void comenzarLectura(){
    tipoelem *e;

    e = siguienteElemento();

    //pedir elementos al analizador léxico hasta que devuelva NULL
    while(e != NULL){
        
        printf("<%s, %d>\n", e->lexema, e->componenteLexico);

        //liberar memoria de la estructura
        free(e->lexema);
        free(e);

        e = siguienteElemento();
    }

}
