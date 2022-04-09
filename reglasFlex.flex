%{
#include "definiciones.h"
#include "errores.h"
#include "tablaSimbolos.h"
%}

    //caracteres
LETRA              [a-zA-Z]|_
DIGITO             [0-9]
DIGITO_BINARIO     [0-1]
DIGITO_OCTAL       [0-7]
DIGITO_HEX         [0-9A-Fa-f]

    //enteros
DIGITOS_DECIMAL     {DIGITO}([_]?{DIGITO})*
DIGITOS_BINARIO     {DIGITO_BINARIO}([_]?{DIGITO_BINARIO})*
DIGITOS_OCTAL       {DIGITO_OCTAL}([_]?{DIGITO_OCTAL})*
DIGITOS_HEX         {DIGITO_HEX}([_]?{DIGITO_HEX})*

LITERAL_DECIMAL     [0-9]([_]?{DIGITOS_DECIMAL})?
LITERAL_BINARIO     0[Bb][_]?{DIGITOS_BINARIO}
LITERAL_OCTAL       0[Oo][_]?{DIGITOS_OCTAL}
LITERAL_HEX         0[Xx][_]?{DIGITOS_HEX}
LITERAL_ENTERO      {LITERAL_DECIMAL}|{LITERAL_BINARIO}|{LITERAL_OCTAL}|{LITERAL_HEX}

    //floats
LITERAL_FLOAT        {LITERAL_DECIMAL_FLOAT}|{LITERAL_FLOAT_HEX}

LITERAL_DECIMAL_FLOAT   {DIGITOS_DECIMAL}\.({DIGITOS_DECIMAL})?({EXPONENTE_DECIMAL})?|{DIGITOS_DECIMAL}{EXPONENTE_DECIMAL}|\.{DIGITOS_DECIMAL}({EXPONENTE_DECIMAL})?
EXPONENTE_DECIMAL       [Ee](\+|-)?{DIGITOS_DECIMAL}

LITERAL_FLOAT_HEX       0[Xx]{MANTISA_HEX}{EXPONENTE_HEX}
MANTISA_HEX             [_]?{DIGITOS_HEX}\.({DIGITOS_HEX})?|[_]?{DIGITOS_HEX}|\.{DIGITOS_HEX}
EXPONENTE_HEX           [Pp](\+|-)?{DIGITOS_DECIMAL}


    //imaginarios
LITERAL_IMAGINARIO     ({DIGITOS_DECIMAL}|{LITERAL_ENTERO}|{LITERAL_FLOAT})i

LITERAL_STRING      ["].+["]

IDENTIFICADOR       {LETRA}({LETRA}|{DIGITO})*

MASIG               \+=
DOSPUNTOSIG         :=
FLECHAIZQ           <-


%option nounput
%option noinput

%%

[ \t\n]+        /* se come los espacios en blanco */
\/\/(.*)        /*comentario de una sola línea*/
\/\*(.*\n)*.*\*\/   /*comentarios de varias líneas*/


{IDENTIFICADOR}         return devolverComponente(yytext); /*Identificador*/

{LITERAL_ENTERO}        return ENTERO;

{LITERAL_FLOAT}         return FLOTANTES;

{LITERAL_STRING}        return CADENAS;

{LITERAL_IMAGINARIO}    return IMAGINARIOS;

{MASIG}                 return MASIGUAL;
{DOSPUNTOSIG}           return DOSPUNTOSIGUAL;
{FLECHAIZQ}             return FLECHAIZQUIERDA;

.                       return (int) *yytext;


{DIGITOS_DECIMAL}\.({DIGITOS_DECIMAL})?[Ee](\+|-)?|{DIGITOS_DECIMAL}[Ee](\+|-)?|\.{DIGITOS_DECIMAL}[Ee](\+|-)?    mostrarError(3);
0[Xx]{MANTISA_HEX}      mostrarError(3);
0[xX][_]?               mostrarError(4);
0[Bb][_]?               mostrarError(5);
0[oO][_]?               mostrarError(6);

%%

//función para pasar como argumento el fichero que contendrá el código que se quiere analizar
void abrirFichero(FILE* fichero){
    if(fichero != NULL) yyin = fichero;
}

//función que permitirá ir obteniendo un puntero a la estructura que contenga el siguiente
//componente léxico o el valor NULL en caso de que se haya alcanzado EOF
tipoelem* siguienteElemento(){
    tipoelem *e;

    e = malloc(sizeof(tipoelem));

    e->componenteLexico = yylex();

    if(e->componenteLexico != 0){
        e->lexema = malloc(yyleng + 1);
        strcpy(e->lexema, yytext);
    }else{
        free(e);
        e = NULL;
    }

    return e;
}

//función que permitirá llamar a yylex_destroy, que liberará los recursos utilizado por el escáner
void terminarAnalisis(){
    yylex_destroy();
}