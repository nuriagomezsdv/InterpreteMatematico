#include <stdio.h>
#include <string.h>

#include "nuriMath.h"
#include "comands.h"
#include "bison.tab.h"

void nuricalc(char *arg){

    // Se imprime mensaje de inicio de la calculadora NuriMath
    printf(AMARILLO
        "\t*--------------------------------------------------------------------*\n"
        "\t*\t\tNURIMATH - CALCULADORA MATEMÁTICA                    *\n"
        "\t*\t\t\t Versión 1.0, 2023.                          *\n"
        "\t*\t\t\t Autora: Nuria.                              *\n"
        "\t*  Intérprete matemático con opción de importar librerías            *\n"
        "\t*  dinámicamente, pudiendo ejecutar cualquier función de las         *\n"
        "\t*  librerías importadas, y de ejecutar ficheros.                     *\n"
        "\t*  Para más información, ejecute el comando help o help().           *\n"
        "\t*--------------------------------------------------------------------*\n"
        "\n"PREDETERMINADO"\n");

    // Comprobamos si se ha llamado con algún argumento
    if (arg != NULL) {
        // Si se ha llamado con la opción -h se llama a la ayuda
        if (strcmp(arg, "-h") == 0) {
            help();
        // Sino se entiende que es un fichero y se llama a load para ejecutarlo
        } else {
            load(arg);
        }
    }
    yyparse();
}