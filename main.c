
#include <stdlib.h>
#include "tablaSimbolos.h"
#include "bison.tab.h"
#include "errores.h"

int main( int argc, char *argv[]) {
    // Llamamos a la inicialización la tabla de símbolos
    inicializarTablaSimbolos();
    // Llamamos al intérprete para que comience
    nuricalc(argv[1]);
    return 0;
}