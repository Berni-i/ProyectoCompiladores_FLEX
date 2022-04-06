{%
include "definiciones.h";
%}

LETRA           [a-zA-z] | "_" 
DIGITO          [0-9]
DIGITO_BINARIO  [0-1]
DIGITO_OCTAL    [0-7]
DIGITO_HEX      [0-9] | [A-F] | [a-f]

%%

{LETRA} ({LETRA} | {DIGITO})*     /*Identificador*/

"{"[^}\n]*"}"   /* se come una linea de comentarios */
[ \t\n]+        /* se come los espacios en blanco */
.               printf( "Caracter no reconocido: %s\n", yytext);

%%