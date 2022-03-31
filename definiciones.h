//definiciones de las palabras clave
#define PACKAGE 500
#define IMPORT 501
#define FUNC 502
#define CHAN 503
#define VAR 504
#define FOR 505
#define RANGE 506
#define GO 507

//definiciones de operadores
#define DOSPUNTOSIGUAL 508
#define MASIGUAL 509
#define FLECHAIZQUIERDA 510
#define RETORNO 511

//definición de literales
#define CADENAS 512
#define ENTERO 513
#define FLOTANTES 514
#define IMAGINARIOS 515

//definición de identificadores
#define ID 516

//definición del tamaño máximo de la cadena
#define TAMCADENA 32

//definición de las estrcuturas con las que trabajará el árbol de la tabla de símbolos
typedef struct {
	char *lexema;
	int componenteLexico;
} tipoelem; 
