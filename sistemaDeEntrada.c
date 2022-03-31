#include <stdio.h>
#include <stdlib.h>

#include "definiciones.h"
#include "errores.h"

FILE *fichero;

// declaración de los dos buffers con un espacio extra para EOF
char b1[TAMCADENA + 1], b2[TAMCADENA + 1];

//contador para asegurar que no se sobrepase el tamaño máximo de lexema
int contador = 0;

// punteros de inicio y delantero
char *inicio = b1, *delantero = b1;

// cargar el buffer correspondiente
void cargarBuffer(char *buffer)
{
    int leidos;

    if(inicio >= buffer && inicio <= (buffer + TAMCADENA)){
        //se perdería el puntero del inicio
        if(buffer == b1){
            //ifs necesarios ya que sino se movería el puntero al inicio del programa
            //ya que tanto inicio como delantero se encuentran apuntando a b1
            if(delantero >= b2 && delantero <= (b2 + TAMCADENA)) inicio = b2;
        }else{
            inicio = b1;
        }
    }

    if (!feof(fichero))
    {

        //leer TAMCADENA elementos de tamaño 1
        leidos = fread(buffer, 1, TAMCADENA, fichero);

        //introducir el caracter EOF porque fread no lo introduce por defecto
        if(leidos < TAMCADENA){
            *(buffer + leidos) = EOF;
        }
    }

}

// iniciar buffer por primera vez y guardar la referencia al fichero
void iniciarBuffer(FILE *id_fichero)
{
    fichero = id_fichero;

    // incluir centinelas
    *(b1 + TAMCADENA) = EOF;
    *(b2 + TAMCADENA) = EOF;

    cargarBuffer(b1);
}

// función que permite pasar cada caracter al analizador sintáctico
char siguienteCaracter()
{
    //leer caracter
    char caracter = *delantero;

    if(contador == TAMCADENA){
        mostrarError(1);
    }

    // comprobar si se encuentra en uno de los fines de fichero
    if (caracter == EOF)
    {
        if (delantero == (b1 + TAMCADENA))
        {
            cargarBuffer(b2);
            delantero = b2 + 1;
            caracter = *b2;

        }
        else if (delantero == (b2 + TAMCADENA))
        {
            cargarBuffer(b1);
            delantero = b1 + 1;
            caracter = *b1;
        }
        else
        {
            delantero += 1;
            return EOF;
        }
    }else{
        delantero += 1;
    }

    //sumar uno al contador de longitud
    contador++;

    return caracter;
}

// devolver la palabra en el momento en que el analizador léxico detecte que se ha llegado
// a un estado final
char *devolverPalabra()
{
    int i = 0, tam = 0;
    char *palabra;

    //reiniciar el contador
    contador = 0;

    //calcular espacio según la distancia que existe entre los punteros
    if(delantero >= b1 && delantero <= (b1+TAMCADENA)){
        if(inicio >= b1 && inicio <= (b1+TAMCADENA)){
            tam = delantero - inicio;
        }else{
            tam = (delantero - b1) + ((b2 + TAMCADENA) - inicio);
        }
    }else{
        if(inicio >= b2 && inicio <= (b2+TAMCADENA)){
            tam = delantero - inicio;
        }else{
            tam = (delantero - b2) + ((b1 + TAMCADENA) - inicio);
        }
    }

    //reservar tamaño para el tamaño de la cadena más el carácter vacío
    palabra = malloc(tam+1);

    //introducir el carácter de fin de cadena
     *(palabra + tam) = '\0';

    //mientras se encuentran separados
    while (inicio != delantero)
    {
        if (*inicio == EOF)
        {
            if(inicio == (b1 + TAMCADENA))
            {
                inicio = b2;
            }
            else if (inicio == (b2 + TAMCADENA))
            {
                inicio = b1;
            }
        }

        //si delantero se encontrase en la primera posición del buffer
        if(inicio == delantero) break;

        //ir introduciendo caracteres uno a uno
        *(palabra + i) = *inicio;

        inicio += 1;
        i++;
    }

    return palabra;
}

// función que permita ignorar caracteres cuando se trate de espacios o comentarios
//llevando inicio a la posición de delantero
void saltarCaracter()
{
    inicio = delantero;
    contador = 0;
}

//función que permite retrasar la posición de delantero como solo se retrasa
//una posición no es necesario considerar posibles cargas de buffer duplicadas
void devolverCaracter()
{
    delantero -= 1;
}