#include "abb.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

abb tablaSimbolos;

//función auxiliar para imprimir la tabla de símbolos
void imprimir(abb A) {
    tipoelem E;

	if(!es_vacio(A)){
	  imprimir(izq(A));
	  leer(A,&E);
          printf("%s, %d\n", E.lexema, E.componenteLexico);
	  imprimir(der(A));
	}
}

//función para imprimir la tabla de símbolos
void imprimirTabla(){
    printf("\n");
    imprimir(tablaSimbolos);
    printf("\n");
}

//función para iniciar la tabla con las palabras reservadas
void inicializarTabla(){

    //declaración de los arrays cn los valores y las palabras
    char nombres[][TAMCADENA] = {"package", "import", "func", "chan", "var", "for", "range", "go"};
    int definiciones[] = {PACKAGE, IMPORT, FUNC, CHAN, VAR, FOR, RANGE, GO};

    //Se crea el árbol binario
    crear(&tablaSimbolos);

    for (int i = 0; i < sizeof(nombres)/TAMCADENA; i++)
    {


        //crear un struct por que contenga lexema y componente léxico
        tipoelem elemento;

        elemento.lexema = malloc(strlen(nombres[i])+1);

        strcpy(elemento.lexema, nombres[i]);
        elemento.componenteLexico = definiciones[i];

        //insertar el struct dentro del árbol
        insertar(&tablaSimbolos, elemento);

    }

}

//función para distinguir entre identificadores y palabras reservadas del lenguaje
int devolverComponente(char *lexema){
    tipoelem e;

    //iniciar el componente léxico a -1 para saber si se encuentra después en la tabla o no
    e.componenteLexico = -1;

    //buscar nodo hace que e apunte a la estructura si está presente en el árbol
    buscar_nodo(tablaSimbolos, lexema, &e);

    if(e.componenteLexico != -1){
        return e.componenteLexico;
    }else{
        e.componenteLexico = ID;

        //se reserva memoria para el lexema y se deja una copia en la tabla de símbolos
        e.lexema = malloc(strlen(lexema)+1);

        strcpy(e.lexema, lexema);

        insertar(&tablaSimbolos, e);

        return ID;
    }
}

//función que permita liberar la memoria refernte a la tabla
void destruirTabla(){
    destruir(&tablaSimbolos);
}
