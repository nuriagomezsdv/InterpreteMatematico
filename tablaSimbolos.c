#include <malloc.h>
#include <string.h>
#include <math.h>
#include <dlfcn.h>

#include "tablaSimbolos.h"
#include "abb.h"
#include "nuriMath.h"
#include "bison.tab.h"
#include "comands.h"
#include "errores.h"

// Definimos el número de palabras reservadas del lenguaje D
#define NUM_CONST_FUN 11

// Definimos la tabla de simbolos como un abb
abb tablaSimbolos;

// Función que crea un componente léxico CONSTANTE
void crearConst(ComponenteLexico *componenteLexico, int compLexico, char *lexema, double valor){
    // Introducimos el componente léxico en el campo componente léxico
    componenteLexico->compLexico = compLexico;
    // Reservamos memoria para el lexema del componente léxico
    componenteLexico->lexema = (char *) malloc(sizeof(char) * strlen(lexema) + 1);
    // Copiamos la palabra reservada en el lexema del componente lexico
    // Al haber reservado memoria para strlen(lexema)+1 caracteres, no hay que preocuparse por el \0, se va a copiar automáticamente
    strcpy(componenteLexico->lexema, lexema);
    componenteLexico->valor.numero = valor;
}

// Función que crea un componente léxico FUNCION
void crearFun(ComponenteLexico *componenteLexico, int compLexico, char *lexema,  double (*funptr)() ){
    // Introducimos el componente léxico en el campo componente léxico
    componenteLexico->compLexico = compLexico;
    // Reservamos memoria para el lexema del componente léxico
    componenteLexico->lexema = (char *) malloc(sizeof(char) * strlen(lexema) + 1);
    // Copiamos la palabra reservada en el lexema del componente lexico
    // Al haber reservado memoria para strlen(lexema)+1 caracteres, no hay que preocuparse por el \0, se va a copiar automáticamente
    strcpy(componenteLexico->lexema, lexema);
    componenteLexico->valor.funptr = funptr;
}

// Función que inicializa la tabla de símbolos
void inicializarTablaSimbolos(){
    // Introducimos las palabras reservadas en la tabla de símbolos
    tipoelem componentesLexicos[NUM_CONST_FUN];
    // Creamos los componentes léxicos de las palabras reservadas
    // Introducimos las palabras reservadas en el array buscando un orden en el que el árbol quede lo más equilibrado posible para que la búsqueda sea más eficiente
    crearConst(&componentesLexicos[0],CONS, "e", 2.7182818284590452353602874);
    crearConst(&componentesLexicos[1],CONS, "pi", 3.141592653589793238462643);

    crearFun(&componentesLexicos[2],COM, "help", help);
    crearFun(&componentesLexicos[3],COM, "ws", ws);
    crearFun(&componentesLexicos[4],COM, "clearws", clearws);
    crearFun(&componentesLexicos[5],COM, "clear", clear);
    crearFun(&componentesLexicos[6],COM, "exit", salir);
    crearFun(&componentesLexicos[7],COMARG, "load", load);
    crearFun(&componentesLexicos[8],COMARG, "echo", hacerEcho);
    crearFun(&componentesLexicos[9],COM, "tabla", tabla);
    crearFun(&componentesLexicos[10],COMARG, "import", importar);

    // Insertamos los componentes léxicos en la tabla de símbolos
    for(int i = 0; i < NUM_CONST_FUN; i++){
        insertar(&tablaSimbolos, componentesLexicos[i]);
    }

}

// Función que busca un lexema en la tabla de símbolos
// Devuelve el componente léxico correspondiente al lexema
// Si el lexema no se encuentra en la tabla de símbolos, devuelve un componente léxico con lexema a NULL
ComponenteLexico buscarComponenteLexico(char* lexema){
    ComponenteLexico compBusqueda = {0, NULL};
    buscar_nodo(tablaSimbolos, lexema, &compBusqueda);
    return compBusqueda;
}

// Función que busca una función en una librería
int buscarFuncion(tipoelem E, char* func, tipoelem *compBusqueda){
    // Buscamos la función en la librería
    void (*fun)() = dlsym(E.valor.funptr, func);
    // Si la función no se encuentra en la librería, no hacemos nada
    if (fun == NULL) {
        return 0;
    }
    // Si la función se encuentra en la librería, creamos el componente léxico de tipo FUN y lo insertamos en la TS
    // Creamos el componente léxico de tipo FUN y lo insertamos en la TS
    crearFun(compBusqueda, FUN, func,  (double (*)())(fun));
    insertar(&tablaSimbolos, *compBusqueda);
    return 1;
}

// Función auxiliar para buscar una librería en la tabla de símbolos
void buscarLibreria(abb A, char* func, tipoelem *compBusqueda, int *encontrada) {
    tipoelem E;
    if (!es_vacio(A)) {
        buscarLibreria(izq(A), func, compBusqueda, encontrada);
        leer(A,&E);
        if(E.compLexico == LIB){
            // Si el componente léxico es de tipo LIB, buscamos la función en la librería
            *encontrada += buscarFuncion(E, func, compBusqueda);
        }
        buscarLibreria(der(A), func, compBusqueda, encontrada);
    }
}


// Función que busca una función en las librerías importadas en la tabla de símbolos
// Si la función se encuentra en alguna de las librerías, crea el componente léxico asociado a la función y lo inserta en la TS
// En ese caso, devuelve el componente léxico correspondiente a la función
// Si la función no se encuentra en ninguna de las librerias de la tabla de símbolos, devuelve un componente léxico con lexema a NULL
ComponenteLexico buscarFuncionLibrerias(char* funcion){
    ComponenteLexico compBusqueda = {0, NULL};
    int encontrada = 0;

    // Recorremos la tabla de símbolos buscando un componente léxico de tipo LIB
    // Si encontramos un componente léxico de tipo LIB, buscamos en la librería correspondiente si existe la función
    // Si existe la función, creamos el componente léxico de tipo FUN y lo insertamos en la TS
    // Devolvemos el componente léxico de tipo FUN
    // Si no existe la función, seguimos buscando en las demás librerías
    // Si no encontramos la función en ninguna de las librerías, devolvemos un componente léxico con lexema a NULL
    buscarLibreria(tablaSimbolos, funcion, &compBusqueda, &encontrada);
    if(encontrada > 1){
        for(int i = 0; i < encontrada; i++){
            suprimir(&tablaSimbolos, compBusqueda);
        }
        compBusqueda.compLexico = 1;
        compBusqueda.lexema = NULL;
    }
    return compBusqueda;
}

// Función que busca una función en una librería
// En caso de no encontrar la función, devuelve un componente léxico con lexema a NULL
ComponenteLexico buscarFuncionLibreria(ComponenteLexico libreria, char* funcion){
    ComponenteLexico compBusqueda = {0, NULL};
    // Si la funcion se encuentra en la tabla de simbolos, la borramos (posiblemente se esta haciendo un cambio de libreria)
    compBusqueda = buscarComponenteLexico(funcion);
    if(compBusqueda.compLexico == FUN){
        suprimir(&tablaSimbolos, compBusqueda);
    }
    buscarFuncion(libreria, funcion, &compBusqueda);
    return compBusqueda;
}


// Funcion que inserta un componente léxico en la tabla de símbolos
void insertarComponenteLexico(ComponenteLexico componenteLexico){
    insertar(&tablaSimbolos, componenteLexico);
}

// Función para modificar el valor de una variable
void modificarValorVariable(char* lexema, double valor){
    ComponenteLexico compBusqueda = {0, NULL};
    // Se devuelve en compBusqueda el componente léxico correspondiente al lexema
    buscar_nodo(tablaSimbolos, lexema, &compBusqueda);
    if(compBusqueda.compLexico == VAR){
        modificar_nodo(&tablaSimbolos, lexema, valor);
    }
}

// Función que cierra las librerías de la tabla de símbolos
void cerrarLibrerias(abb A){
    tipoelem E;
    if (!es_vacio(A)) {
        cerrarLibrerias(izq(A));
        leer(tablaSimbolos,&E);
        if(E.compLexico == LIB){
            dlclose(E.valor.funptr);
        }
        cerrarLibrerias(der(A));
    }
}

// Funcion que limpia la memoria utilizada por la tabla de símbolos
void limpiarMemoriaTablaSimbolos(){
    // Cerramos las librerías abiertas de la tabla de símbolos
    //cerrarLibrerias(tablaSimbolos);
    // Liberamos la memoria utilizada por la tabla de símbolos
    destruir(&tablaSimbolos);
}

// Función auxiliar para imprimir los elementos de la tabla de símboloss
void imprimirElemento(abb A) {
    tipoelem E;
    if (!es_vacio(A)) {
        imprimirElemento(izq(A));
        leer(A,&E);
        printf(
                "\t*--------------------------------------------------------------------*\n");
        if(E.compLexico == VAR)
            printf(    "\t*     %-17s|          VAR           |    %-16lf*\n"
            , E.lexema, E.valor.numero);
        else if(E.compLexico == CONS)
            printf(    "\t*     %-17s|          CONS          |    %-16lf*\n"
            , E.lexema, E.valor.numero);
        else if(E.compLexico == FUN)    
            printf(    "\t*     %-17s|          FUN           |     -              *\n"
                , E.lexema);
        else if(E.compLexico == COM)    
            printf(    "\t*     %-17s|          COM           |     -              *\n"
                , E.lexema);
        else if(E.compLexico == COMARG)    
            printf(    "\t*     %-17s|          COMARG        |     -              *\n"
                , E.lexema);
        else if(E.compLexico == LIB)
            printf(    "\t*     %-17s|          LIB           |     -              *\n"
                , E.lexema);
        imprimirElemento(der(A));
    }
}

// Función que imprime el contenido de la tabla de símbolos
void imprimirTablaSimbolos(){
    // Imprimimos la tabla de símbolos
    printf(AMARILLO
    "\t*--------------------------------------------------------------------*\n"
    "\t*\t\t\t    TABLA DE SÍMBOLOS                        *\n"
    "\t*--------------------------------------------------------------------*\n"
    "\t*\t LEXEMA        |          TIPO          |        VALOR       *\n");
    imprimirElemento(tablaSimbolos);
    printf(AMARILLO
           "\t*--------------------------------------------------------------------*\n"PREDETERMINADO"\n");
}

// Función auxiliar para imprimir las variables de la tabla de símboloss
void imprimirVariable(abb A, int *contador) {
    tipoelem E;
    if (!es_vacio(A)) {
        imprimirVariable(izq(A), contador);
        leer(A,&E);
        if(E.compLexico == VAR){
        (*contador)++;
        printf(
                "\t*--------------------------------------------------------------------*\n"
                "\t*     %-28s|    %-30lf*\n"
                , E.lexema, E.valor.numero);
        }
        imprimirVariable(der(A), contador);
    }
}

// Funcón que imprime el workspace (las variables declaradas) de la tabla de simbolos
void imprimirWorkspace(){
    int cont = 0;
    printf(AMARILLO
           "\t*--------------------------------------------------------------------*\n"
           "\t*\t\t\t      WORKSPACE                              *\n"
           "\t*--------------------------------------------------------------------*\n"
           "\t*\t\t     VARIABLE     |      VALOR                       *\n"
          );
    imprimirVariable(tablaSimbolos, &cont);
    if(cont == 0){
        printf(AMARILLO
        "\t*--------------------------------------------------------------------*\n"
        "\t*\t\t\t        VACÍO                                *\n");

    }
    printf(AMARILLO
           "\t*--------------------------------------------------------------------*\n"PREDETERMINADO"\n");
}


// Función auxiliar para eliminar las variables de la tabla de símbolos
void eliminarVariable(abb A){
    tipoelem E;
    if (!es_vacio(A)) {
        eliminarVariable(izq(A));
        eliminarVariable(der(A));
        leer(A,&E);
        if(E.compLexico == VAR)
            suprimir(&tablaSimbolos,E);
        
    }
}


// Funcion que elimina el worksapce (las variables declaradas) de la tabla de simbolos
void eliminarWorkspace(){
    eliminarVariable(tablaSimbolos);
}