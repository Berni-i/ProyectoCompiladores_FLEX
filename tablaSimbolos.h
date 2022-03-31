//permite reservar memoria para la tabla y cargar las palabras reservadas
void inicializarTabla();

//permite liberar la memoria de la tabla
void destruirTabla();

//permite distinguir entre identificadores y palabras reservadas
int devolverComponente(char *lexema);

//permite imprimir la tabla de símbolos
void imprimirTabla();