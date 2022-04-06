#include <string.h>
#include <stdlib.h>

#include "definiciones.h"
#include "sistemaDeEntrada.h"
#include "tablaSimbolos.h"
#include "errores.h"

void cadenaAlfanumerica(char caracter, tipoelem *e);

void numeros(char caracter, tipoelem *e);

void operadoresVariosDigitos(char caracter, tipoelem *e);

void puntoFlotante(char caracter, tipoelem *e);

void cadenas(tipoelem *e);

void flotanteHexadecimal(char caracter, tipoelem *e);

// función que devolverá un puntero a una estructura tipoelem conteniendo el siguiente lexema
// y su correspondiente componente léxico.
tipoelem *siguienteElemento()
{
    tipoelem *e;

    // leer caracter
    char caracter = siguienteCaracter();

    //detectar en primer lugar los espacios, comentarios o el caracter de fin de fichero
    while (caracter == '/' || caracter == 32 || caracter == '\n' || caracter == EOF)
    {
        // si es el final de fichero devolver null
        if (caracter == EOF)
        {
            return NULL;
        }

        //saltar los espacios
        while (caracter == 32 || caracter == '\n')
        {
            saltarCaracter();
            caracter = siguienteCaracter();
        }

        // detectar comentarios y omitirlos también
        if (caracter == '/')
        {
            caracter = siguienteCaracter();

            if (caracter == '/')
            {
                // saltar hasta el final de línea
                while (caracter != '\n')
                {
                    caracter = siguienteCaracter();
                    saltarCaracter();
                }
            }
            else if (caracter == '*')
            {
                while (caracter != EOF)
                {
                    caracter = siguienteCaracter();
                    saltarCaracter();

                    // saltar hasta que encuentre un */
                    if (caracter == '*')
                    {
                        caracter = siguienteCaracter();
                        saltarCaracter();

                        if (caracter == '/')
                        {
                            caracter = siguienteCaracter();
                            saltarCaracter();
                            break;
                        }
                    }
                }
            }
            else
            {
                // reservar memoria después de comprobar que no es un comentario
                e = malloc(sizeof(tipoelem));

                // devolver / si no se trata de un comentario
                caracter = '/';

                devolverCaracter();

                e->lexema = devolverPalabra();
                e->componenteLexico = caracter;
                saltarCaracter();

                return e;
            }
        }
    }

    // reservar memoria después de saltar comentarios y espacios
    e = malloc(sizeof(tipoelem));

    // comprobar si se trata de una cadena alfanumérica
    // un caracter del alfabeto o una barra baja
    if (((caracter >= 65 && caracter <= 90) || (caracter >= 97 && caracter <= 122)) || caracter == '_')
    {
        // autómata de cadenas alfanuméricas
        cadenaAlfanumerica(caracter, e);
    }
    else if (caracter == '.' || (caracter >= 48 && caracter <= 57))
    { // números empiezan por punto, 1 o 0x
        // autómata de números
        numeros(caracter, e);
    }
    else if (caracter == 58 || caracter == 60 || caracter == 43)
    { // considerar para el código :=, <-, +=,
        // autómata para operadores de varios símbolos
        operadoresVariosDigitos(caracter, e);
    }
    else if (caracter == '"')
    {
        cadenas(e);
    }
    else
    { // lexemas que solo contengan un caracter como +, -, )...
        e->lexema = devolverPalabra();
        e->componenteLexico = caracter;
        saltarCaracter();
    }

    return e;
}

// función para leer cadenas alfanuméricas
void cadenaAlfanumerica(char caracter, tipoelem *e)
{

    char leido = siguienteCaracter();

    while ((leido >= 65 && leido <= 90) || (leido >= 97 && leido <= 122) || (leido >= 48 && leido <= 57) || leido == 95)
    {
        leido = siguienteCaracter();
    }

    // devolver el último caracter que se haya leido
    devolverCaracter();

    // actualizar elemento
    e->lexema = devolverPalabra();

    // preguntar a la tabla de símbolos si se trata de un identificador o de una palabra reservada
    e->componenteLexico = devolverComponente(e->lexema);
}

// función para leer números
void numeros(char caracter, tipoelem *e)
{

    char leido = siguienteCaracter();

    if (caracter == '.')
    {
        // PUNTO FLOTANTE EMPEZANDO POR PUNTO
        if ((leido >= 48 && leido <= 57) || leido == 'e' || leido == 'E')
        {
            puntoFlotante(leido, e);
        }
        else
        {
            // solo se trata de un punto
            devolverCaracter();
            e->lexema = devolverPalabra();
            e->componenteLexico = caracter;
        }
    }
    else if (caracter >= 48 && caracter <= 57)
    {

        // Detectar hexadecimal
        if ((caracter == '0') && (leido == 'x' || leido == 'X'))
        {
            // HEXADECIMAL
            leido = siguienteCaracter();

            if (leido == '.')
            {
                // numero tipo 0x.
                flotanteHexadecimal(leido, e);
            }
            else if ((leido >= 48 && leido <= 57) || (leido >= 97 && leido <= 102) || (leido >= 65 && leido <= 70))
            {
                // leer todos los dígitos hexadecimales
                while ((leido >= 48 && leido <= 57) || (leido >= 97 && leido <= 102) || (leido >= 65 && leido <= 70))
                {
                    leido = siguienteCaracter();
                }

                if (leido == '.')
                {
                    // numero tipo 0x1a2b.
                    flotanteHexadecimal(leido, e);
                }
                else if (leido == 'p' || leido == 'P')
                {
                    // hexadecimal con exponente
                    leido = siguienteCaracter();

                    if (leido == '+' || leido == '-')
                    {
                        leido = siguienteCaracter();
                    }

                    // comprobar si existe un exponente
                    if ((leido >= 48 && leido <= 57) || (leido >= 97 && leido <= 102) || (leido >= 65 && leido <= 70))
                    {
                        while ((leido >= 48 && leido <= 57) || (leido >= 97 && leido <= 102) || (leido >= 65 && leido <= 70))
                        {
                            leido = siguienteCaracter();
                        }

                        // comprobar si es imaginario o no
                        if (leido == 'i')
                        {
                            //no es necesario devolver el carácter ya que la i forma parte del lexema
                            e->lexema = devolverPalabra();
                            e->componenteLexico = IMAGINARIOS;
                        }
                        else
                        {
                            devolverCaracter();

                            e->lexema = devolverPalabra();
                            e->componenteLexico = FLOTANTES;
                        }
                    }
                    else
                    {
                        // número sin exponente después de la p
                        mostrarError(3);
                        devolverPalabra();
                    }
                }
                else if (leido == 'i')
                {
                    // numero imaginario sin exponente ni parte decimal
                    e->lexema = devolverPalabra();
                    e->componenteLexico = IMAGINARIOS;
                }
                else
                {
                    devolverCaracter();

                    // actualizar elemento
                    e->lexema = devolverPalabra();
                    // solo cifras hexadecimales
                    e->componenteLexico = ENTERO;
                }
            }
            else
            {
                // solo aparece el 0x
                mostrarError(4);
                devolverPalabra();
            }
        }
        // Detectar binario
        else if ((caracter == '0') && (leido == 'b' || leido == 'B'))
        {
            // BINARIO
            leido = siguienteCaracter();

            while (leido == '0' || leido == '1')
            {
                leido = siguienteCaracter();
            }

            // comprobar si es imaginario
            if (leido == 'i')
            {
                // actualizar elemento
                e->lexema = devolverPalabra();
                e->componenteLexico = IMAGINARIOS;
            }
            else
            {
                devolverCaracter();

                // actualizar elemento
                e->lexema = devolverPalabra();
                e->componenteLexico = ENTERO;
            }
        }
        // Detetar octal
        else if ((caracter == '0') && (leido == 'o' || leido == 'O'))
        {
            // BINARIO
            leido = siguienteCaracter();

            while (leido >= 48 && leido <= 55)
            {
                leido = siguienteCaracter();
            }

            // comprobar si es imaginario
            if (leido == 'i')
            {
                // actualizar elemento
                e->lexema = devolverPalabra();
                e->componenteLexico = IMAGINARIOS;
            }
            else
            {
                devolverCaracter();

                // actualizar elemento
                e->lexema = devolverPalabra();
                e->componenteLexico = ENTERO;
            }
        }
        else
        {

            // EMPEZANDO POR DÍGITOS
            while ((leido >= 48 && leido <= 57))
            {
                leido = siguienteCaracter();
            }

            if (leido == '.')
            {
                // PUNTO FLOTANTE
                puntoFlotante(leido, e);
            }
            else if (leido == 'i')
            {
                // IMAGINARIO
                e->lexema = devolverPalabra();
                e->componenteLexico = IMAGINARIOS;
            }
            else if (leido == 'e' || leido == 'E')
            {
                // NÚMERO CON EXPONENTE
                leido = siguienteCaracter();

                if (leido == '+' || leido == '-')
                {
                    leido = siguienteCaracter();
                }

                if (leido >= 48 && leido <= 57)
                {
                    while (leido >= 48 && leido <= 57)
                    {
                        leido = siguienteCaracter();
                    }
                }
                else
                {
                    mostrarError(3);
                }

                if (leido == 'i')
                {
                    // actualizar elemento
                    e->lexema = devolverPalabra();
                    e->componenteLexico = IMAGINARIOS;
                }
                else
                {
                    devolverCaracter();

                    // actualizar elemento
                    e->lexema = devolverPalabra();
                    e->componenteLexico = FLOTANTES;
                }
            }
            else
            {
                // ENTERO SIMPLE
                devolverCaracter();

                e->lexema = devolverPalabra();
                e->componenteLexico = ENTERO;
            }
        }
    }
}

// función para comprobar operadores formados por más de un carácter
void operadoresVariosDigitos(char caracter, tipoelem *e)
{
    char leido;

    //:=
    if (caracter == ':')
    {
        leido = siguienteCaracter();

        if (leido == '=')
        {
            e->lexema = devolverPalabra();
            e->componenteLexico = DOSPUNTOSIGUAL;
        }
        else
        {
            devolverCaracter();

            e->lexema = devolverPalabra();
            e->componenteLexico = ':';
        }
    }

    //+=
    if (caracter == '+')
    {
        leido = siguienteCaracter();

        if (leido == '=')
        {
            e->lexema = devolverPalabra();
            e->componenteLexico = MASIGUAL;
        }
        else
        {
            devolverCaracter();

            e->lexema = devolverPalabra();
            e->componenteLexico = '+';
        }
    }

    //<-
    if (caracter == '<')
    {
        leido = siguienteCaracter();

        if (leido == '-')
        {
            e->lexema = devolverPalabra();
            e->componenteLexico = FLECHAIZQUIERDA;
        }
        else
        {
            devolverCaracter();

            e->lexema = devolverPalabra();
            e->componenteLexico = '<';
        }
    }
}

// función para leer la parte derecha del punto en un número
void puntoFlotante(char caracter, tipoelem *e)
{

    char leido = siguienteCaracter();

    // leer números en formato decimal
    while (leido >= 48 && leido <= 57)
    {
        leido = siguienteCaracter();
    }

    // comprobar exponente
    if (leido == 'E' || leido == 'e')
    {
        leido = siguienteCaracter();

        if (leido == '+' || leido == '-')
        {
            leido = siguienteCaracter();
        }

        if (leido >= 48 && leido <= 57)
        {
            while (leido >= 48 && leido <= 57)
            {
                leido = siguienteCaracter();
            }
        }
        else
        {
            mostrarError(3);
        }
    }

    // comprobar si es imaginario
    if (leido == 'i')
    {
        e->lexema = devolverPalabra();
        e->componenteLexico = IMAGINARIOS;
    }
    else
    {
        devolverCaracter();

        e->lexema = devolverPalabra();
        e->componenteLexico = FLOTANTES;
    }
}

// función para detectar cadenas
void cadenas(tipoelem *e)
{
    char leido;

    do
    {
        // leer caracteres hasta que se encuentre otras comillas o EOF
        leido = siguienteCaracter();

        if (leido == '\\')
        {
            leido = siguienteCaracter();

            if (leido == '"')
            {
                continue;
            }
        }

        if (leido == '"')
            break;

    } while (leido != EOF);

    // actualizar elemento
    e->lexema = devolverPalabra();
    e->componenteLexico = CADENAS;
}

// función para leer la parte derecha del punto en números hexadecimales
void flotanteHexadecimal(char caracter, tipoelem *e)
{
    char leido = siguienteCaracter();

    while ((leido >= 48 && leido <= 57) || (leido >= 97 && leido <= 102) || (leido >= 65 && leido <= 70))
    {
        leido = siguienteCaracter();
    }

    if (leido == 'p' || leido == 'P')
    {
        // NÚMERO CON EXPONENTE
        leido = siguienteCaracter();

        if (leido == '+' || leido == '-')
        {
            leido = siguienteCaracter();
        }

        if ((leido >= 48 && leido <= 57) || (leido >= 97 && leido <= 102) || (leido >= 65 && leido <= 70))
        {
            while ((leido >= 48 && leido <= 57) || (leido >= 97 && leido <= 102) || (leido >= 65 && leido <= 70))
            {
                leido = siguienteCaracter();
            }
        }
        else
        {
            mostrarError(3);
        }
    }

    // comprobar si es imaginario
    if (leido == 'i')
    {
        // actualizar elemento
        e->lexema = devolverPalabra();
        e->componenteLexico = IMAGINARIOS;
    }
    else
    {
        devolverCaracter();

        // actualizar elemento
        e->lexema = devolverPalabra();
        e->componenteLexico = FLOTANTES;
    }
}
