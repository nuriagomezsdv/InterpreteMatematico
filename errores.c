
#include "stdio.h"
#include "errores.h"
#include "nuriMath.h"

// Función que imprime los distintos errores en función del código de error
void imprimirError(int error) {
    switch (error) {
        case APERTURA_FICHERO:
            printf(ROJO"ERROR: No se puede realizar la operación\nNo se ha podido abrir el fichero."PREDETERMINADO"\n\n");
            break;
        case MEMORIA_INSUFICIENTE:
            printf(ROJO"ERROR: No se puede realizar la operación\nMemoria insuficiente."PREDETERMINADO"\n\n");
            break;
        case FICHERO_NO_PROPORCIONADO:
            printf(ROJO"ERROR: No se puede realizar la operación\nDebe indicar el nombre del fichero a analizar.\n ./nuriMath [FICHERO_CODIGO_FUENTE.d]"PREDETERMINADO"\n\n");
            break;
        case LEXEMA_DESCONOCIDO:
            printf(ROJO"ERROR: No se puede realizar la operación\nLexema desconocido."PREDETERMINADO"\n\n"PREDETERMINADO"\n\n");
            break;
        case ASIGNACION_CONSTANTE:
            printf(ROJO"ERROR: No se puede realizar la operación\nNo se puede asignar un valor a una constante."PREDETERMINADO"\n\n");
            break;
        case DETECTADO_NAN:
            printf(ROJO"ERROR: No se puede realizar la operación\nNaN detectado."PREDETERMINADO"\n\n");
            break;
        case MODULO_CERO:
            printf(ROJO"ERROR: No se puede realizar la operación:\nMódulo cero."PREDETERMINADO"\n\n");
            break;
        case VARIABLE_NO_DEFINIDA:
            printf(ROJO"ERROR: No se puede realizar la operación:\nVariable no definida."PREDETERMINADO"\n\n");
            break;
        case DIVISION_CERO:
            printf(ROJO"ERROR: No se puede realizar la operación:\nDivisión por cero."PREDETERMINADO"\n\n");
            break;
        case NO_ADMITE_ARGUMENTO:
            printf(ROJO"ERROR: No se puede realizar la operación:\nEl comando indicado no admite argumentos."PREDETERMINADO"\n\n");
            break;
        case ARGUMENTO_NO_INDICADO:
            printf(ROJO"ERROR: No se puede realizar la operación:\nArgumento no indicado."PREDETERMINADO"\n\n");
            break;
        case ARGUMENTO_NO_VALIDO:
            printf(ROJO"ERROR: No se puede realizar la operación:\nArgumento no válido."PREDETERMINADO"\n\n");
            break;
        case APERTURA_LIBRERIA:
            printf(ROJO"ERROR: No se puede realizar la operación:\nNo se ha podido abrir la librería."PREDETERMINADO"\n\n");
            break;
        case OBTENCION_FUNCION:
            printf(ROJO"ERROR: No se puede realizar la operación:\nNo se ha podido obtener la función."PREDETERMINADO"\n\n");
            break;
        case FUNCION_VARIAS_LIBRERIAS:
            printf(ROJO"ERROR: No se puede realizar la operación:\nSe ha encontrado la función en varias librerías.\n" 
            "Indique de que librería desea obtener la función en esta sesión: LIBRERIA/FUNCION()"PREDETERMINADO"\n\n");
            break;
        case LIBRERIA_NO_IMPORTADA:
            printf(ROJO"ERROR: No se puede realizar la operación:\nLa librería indicada no ha sido importada."PREDETERMINADO"\n\n");
        default:
            printf(ROJO"ERROR DESCONOCIDO."PREDETERMINADO"\n\n");
            break;
    }

}

/* Función llamada por yyparse del analizador sintactico ante un error sintactico */
void yyerror(char *s) {
    printf(ROJO"ERROR SINTÁCTICO: %s\n"PREDETERMINADO"\n", s);
}
