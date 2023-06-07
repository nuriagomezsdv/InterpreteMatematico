#ifndef INTERPRETEMATEMATICO_COMANDS_H
#define INTERPRETEMATEMATICO_COMANDS_H

// Funciones del sistema

// Función que limpia la pantalla
double clear();

// Función para salir del programa
double salir();

// Función que imprime la ayuda del intérprete
double help();

// Fucnión que asigna a echo el valor recibido [on/off]
double hacerEcho(char* accion);

// Función que imprime el workspace
double ws();

// Función que limpia el workspace
double clearws();

// Función que imprime la tabla de símbolos
double tabla();

// Función que ejecuta el fichero .n que recibe como parámetro
double load(char *fichero);

// Función para importar una librería en tiempo de ejcución
double importar(char *libreria);

#endif //INTERPRETEMATEMATICO_COMANDS_H