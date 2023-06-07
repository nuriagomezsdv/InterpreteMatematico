
#ifndef COMPILADORD_ERRORES_H
#define COMPILADORD_ERRORES_H

// Definimos distintos errores
#define APERTURA_FICHERO 100
#define MEMORIA_INSUFICIENTE 101
#define FICHERO_NO_PROPORCIONADO 102
#define LEXEMA_DESCONOCIDO 103
#define ASIGNACION_CONSTANTE 104
#define MODULO_CERO 105
#define VARIABLE_NO_DEFINIDA 106
#define DETECTADO_NAN 107
#define DIVISION_CERO 108
#define NO_ADMITE_ARGUMENTO 109
#define ARGUMENTO_NO_INDICADO 110
#define ARGUMENTO_NO_VALIDO 111
#define APERTURA_LIBRERIA 112
#define OBTENCION_FUNCION 113
#define FUNCION_VARIAS_LIBRERIAS 114
#define LIBRERIA_NO_IMPORTADA 115

// Función que imprime los distintos errores según el código de error
void imprimirError(int error);

/* Función llamada por yyparse del analizador sintactico ante un error sintactico */
void yyerror(char* s);

#endif //COMPILADORD_ERRORES_H
