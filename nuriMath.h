#ifndef INTERPRETEMATEMATICO_NURIMATH_H
#define INTERPRETEMATEMATICO_NURIMATH_H

// Definimos colores para la consola
#define ROJO "\x1B[31m"
#define VERDE "\x1B[32m"
#define AMARILLO "\x1B[33m"
#define PREDETERMINADO "\x1B[0m"


// Estructura de componente léxico
typedef struct {
    int compLexico;
    char *lexema;
    union{
        double numero;
        double (*funptr)();
    } valor;
} ComponenteLexico;

// Función para invocar al intérprete matemático
void nuricalc(char *arg);

#endif //INTERPRETEMATEMATICO_NURIMATH_H