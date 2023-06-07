
#ifndef COMPILADORD_TABLASIMBOLOS_H
#define COMPILADORD_TABLASIMBOLOS_H
#include "nuriMath.h"

// Función que inicializa la tabla de símbolos
void inicializarTablaSimbolos();

// Función que busca un lexema en la tabla de símbolos y devuelve su componente léxico
// Si el lexema no se encuentra en la tabla de símbolos, devuelve un componente léxico con lexema a NULL
ComponenteLexico buscarComponenteLexico(char *lexema);

// Función que busca una función en las librerías importadas en la tabla de símbolos
// Si la función se encuentra en alguna de las librerías, crea el componente léxico asociado a la función y lo inserta en la TS
// En ese caso, devuelve el componente léxico correspondiente a la función
// Si la función no se encuentra en ninguna de las librerias de la tabla de símbolos, devuelve un componente léxico con lexema a NULL
ComponenteLexico buscarFuncionLibrerias(char* lexema);

// Función que busca una función en una librería indicada
// En caso de no encontrar la función, devuelve un componente léxico con lexema a NULL
ComponenteLexico buscarFuncionLibreria(ComponenteLexico libreria, char* funcion);

// Función que inserta un lexema en la tabla de símbolos
void insertarComponenteLexico(ComponenteLexico componenteLexico);

// Función para modificar el valor de una variable
void modificarValorVariable(char* lexema, double valor);

// Función para imprimir el Workspace
void imprimirWorkspace();

// Función para eliminar el Workspace
void eliminarWorkspace();

// Función que limpia la memoria utilizada por la tabla de símbolos
void limpiarMemoriaTablaSimbolos();

// Función que imprime el contenido de la tabla de símbolos
void imprimirTablaSimbolos();

#endif //COMPILADORD_TABLASIMBOLOS_H
