#include <stdio.h>
#include <dlfcn.h>

#include "comands.h"
#include "errores.h"
#include "bison.tab.h"
#include "lex.yy.h"
#include "tablaSimbolos.h"

// Función para limpiar el workspace
double clearws(){
    eliminarWorkspace();
    printf (VERDE"\tWorkspace vaciado correctamente."PREDETERMINADO"\n\n");
    return 0;
}

// Función para limpiar la pantalla
double clear(){
    system("clear");
    return 0;
}

// Función que sale del programa
double salir(){
    limpiarMemoriaTablaSimbolos();
    yylex_destroy();
    printf (VERDE"\tSaliendo de NuriMath..."PREDETERMINADO"\n\n");
    exit(EXIT_SUCCESS);
}

// Función que imprime la ayuda del interprete
double help(){
    printf(AMARILLO
        "\t*--------------------------------------------------------------------*\n"
        "\t*\t\tNURIMATH - CALCULADORA MATEMÁTICA                    *\n"
        "\t*\t\t\t Versión 1.0, 2023.                          *\n"
        "\t*\t\t\t Autora: Nuria.                              *\n"
        "\t*""\033[1m"" DESCRIPCIÓN    ""\033[0m"AMARILLO"                                                    *\n"
AMARILLO"\t*  Intérprete matemático con opción de importar librerías            *\n"
        "\t*  dinámicamente, pudiendo ejecutar cualquier función de las         *\n"
        "\t*  librerías importadas, y de ejecutar ficheros.                     *\n"
        "\t*  En caso de ejecutar una función que se encuentre en varias de     *\n"
        "\t*  las librerías importadas, el intérprete solicitará la elección    *\n"
        "\t*  de la librería que se desea utilizar para ejecutar dicha función  *\n"
        "\t*  llamando por primera vez a la función de la siguiente forma:      *\n"
        "\t*  [LIBRERIA]/[FUNCION]()                                            *\n"
        "\t*  Esta elección se mantendrá durante toda la sesión a menos que se  *\n"
        "\t*  indique un cambio de librería para la función, esto se realiza    *\n"
        "\t*  llamando a la función de la siguiente forma:                      *\n"
        "\t*  [NUEVA_LIBRERIA]/[FUNCION]() .                                    *\n"
        "\t*  Realiza todo tipo de operaciones aritméticas : sumas, restas,     *\n"
        "\t*  divisiones, multiplicaciones, potencias y módulos.                *\n"
        "\t*  Además, permite crear variables, asignándoles un valor, y         *\n"
        "\t*  posteriormente trabajar con ellas.                                *\n"
        "\t*  En caso de escribir ';' al final de la operación que se desea     *\n"
        "\t*  realizar no se escribirá el valor de dicha operación.             *\n"
        "\t*  Por último, admite en los ficheros la aparición de comentarios    *\n"
        "\t*  con los siguientes formatos:                                      *\n"
        "\t*  - Comentarios de bloque : /* Comentario */                        *\n"
        "\t*  - Comentarios anidados :  /+ Comentario +/                        *\n"
        "\t*  - Comentarios de línea:   // Comentario                           *\n"
        "\t*                                                                    *\n"
        "\t*""\033[1m"" COMANDOS    ""\033[0m"AMARILLO"                                                       *\n"
        "\t*  Cabe destacar que los siguientes comandos se pueden ejecutar con  *\n"
        "\t*  o sin paréntesis:                                                 *\n"
        "\t*  comando   - comando [ARGUMENTO]                                   *\n"
        "\t*  comando() - comando([ARGUMENTO])                                  *\n"
        "\t*                                                                    *\n"
        "\t*  load [FICHERO.n]  -   Carga un fichero de entrada y lo ejecuta.   *\n"
        "\t*  echo [on/off]     -   Activa o desactiva el modo echo. Con echo   *\n"
        "\t*                        activado se imprimirán los resultados.      *\n"
        "\t*  import [LIBRERIA] -   Importa una librería, de manera que permite *\n"
        "\t*                        utilizar las funciones de dicha librería.   *\n"
        "\t*                        Se importa a través de su fichero .so . Si  *\n"
        "\t*                        se indica solo el nombre, se buscará por    *\n"
        "\t*                        orden en las rutas /lib y /usr/lib . Sin    *\n"
        "\t*                        embargo, se pueden importar librerias       *\n"
        "\t*                        indicando la ruta completa de su            *\n"
        "\t*                        fichero .so .                               *\n"
        "\t*  ws                -   Muestra el workspace.                       *\n"
        "\t*  tabla             -   Muestra la tabla de símbolos.               *\n"
        "\t*  clear             -   Limpia la pantalla.                         *\n"
        "\t*  clearws           -   Limpia el workspace.                        *\n"
        "\t*  tabla             -   Muestra la tabla de símbolos.               *\n"
        "\t*  exit              -   Sale del programa.                          *\n"
        "\t*  help              -   Muestra la ayuda.                           *\n"
        "\t*--------------------------------------------------------------------*\n"
        "\n"PREDETERMINADO"\n");
    return 0;
}

// Función que cambia el echo al valor recibido
double hacerEcho(char *accion){
    // Si se recibe on, se activa el echo (se pone a 1)
    if(strcmp(accion, "on") == 0){
        cambiarEcho(1);
     // Si se recibe off, se desactiva el echo (se ponea 0)
    }else if(strcmp(accion, "off") == 0){
        cambiarEcho(0);
    }else{
        imprimirError(ARGUMENTO_NO_VALIDO);
    }
    return 0;
}  

// Función que imprime el workspace
double ws(){
    imprimirWorkspace();
    return 0;
}

// Función que imprime la tabla de símbolos
double tabla(){
    imprimirTablaSimbolos();
    return 0;
}

// Función para cargar un fichero de entrada
double load(char *fichero){
    // Se abre el fichero de entrada
    yyin = fopen(fichero, "r");
    // Se comprueba que el fichero se ha abierto correctamente
    if (yyin == NULL) {
        imprimirError(APERTURA_FICHERO);
        yyin = stdin;
    }else{
        cambiarScript(1);
        yyparse();
    }
    return 0;
}

// Función para importar librerías en tiempo de ejecución
double importar(char *libreria){
    char *elementoPath, *nombreLibreria;
    // Se abre la librería
    void *handle = dlopen(libreria, RTLD_LAZY);
    // Se comprueba que la librería se ha abierto correctamente
    if (handle == NULL) {
        imprimirError(APERTURA_LIBRERIA);
        return 0;
    }

    // Se separa por / el nombre por si es una ruta
    elementoPath = strtok(libreria, "/");
    
    // Se recorre hasta el ultimo elemento de la ruta, que sera el nombre de la libreria
    while (elementoPath != NULL) {
        nombreLibreria = elementoPath;
        elementoPath = strtok(NULL, "/");
    }

    // Añadimos el componente lexico libreria a la tabla de símbolos
    ComponenteLexico c;
    c.lexema = strdup(nombreLibreria);
    c.compLexico = LIB;
    c.valor.funptr = handle;

    insertarComponenteLexico(c);

    printf (VERDE"\tLibrería %s importada correctamente."PREDETERMINADO"\n\n", nombreLibreria);

    return 0;
}