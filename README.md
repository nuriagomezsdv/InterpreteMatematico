# Intérprete matemático

## Introducción
Este intérprete matemático se ha realizado para aprender de forma didáctica la utilización de las herramientas `Flex` y `Bison` en un contexto específico.

El intérprete tiene definidas dos constantes: `pi` y `e`. Las constantes no se pueden modificar.

Las operaciones permitidas por defecto son las siguientes: suma (`+`), resta (`-`), multiplicación (`*`), división (`/`), módulo (`%`) y potencia (`^`). De todos modos, se pueden añadir más operaciones mediante la importación de librerías. Por ejemplo: para importar la librería matemática se escribe el comando `import libm.so.6`.

## Descripción          
 Intérprete matemático con opción de importar librerías dinámicamente, pudiendo ejecutar cualquier función de las librerías importadas, y de ejecutar ficheros. En caso de ejecutar una función que se encuentre en varias de las librerías importadas, el intérprete solicitará la elección de la librería que se desea utilizar para ejecutar dicha función llamando por primera vez a la función de la siguiente forma: `[LIBRERIA]/[FUNCION]()`
Esta elección se mantendrá durante toda la sesión a menos que se indique un cambio de librería para la función. Esto se realiza llamando a la función de la siguiente forma: `[NUEVA_LIBRERIA]/[FUNCION]()`.   

Realiza todo tipo de operaciones aritméticas : sumas, restas,divisiones, multiplicaciones, potencias y módulos. Además, permite crear variables, asignándoles un valor, y posteriormente trabajar con ellas.   En caso de escribir `;` al final de la operación que se desea realizar no se escribirá el valor de dicha operación.
Por último, admite en los ficheros la aparición de comentarios con los siguientes formatos:
* __Comentarios de bloque :__  `/* Comentario */`    
* __Comentarios anidados :__  `/+ Comentario +/`  
* __Comentarios de línea:__  `// Comentario`   

## Comandos                                                          
Cabe destacar que los siguientes comandos se pueden ejecutar con o sin paréntesis:                                                 
*  `comando`   - `comando [ARGUMENTO]`                                  
*  `comando()` - `comando([ARGUMENTO])`                                                                                                      
*  `load [FICHERO.n]`  -   Carga un fichero de entrada y lo ejecuta.   
*  `echo [on/off]`     -   Activa o desactiva el modo echo. Con echo activado se imprimirán los resultados.      
*  `import [LIBRERIA]` -   Importa una librería, de manera que permite utilizar las funciones de dicha librería. Se importa a través de su fichero `.so` . Si se indica solo el nombre, se buscará por orden en las rutas /lib y /usr/lib . Sin embargo, se pueden importar librerias indicando la ruta completa de su fichero `.so` .  
*  `ws`                -   Muestra el workspace.                       
*  `tabla`           -   Muestra la tabla de símbolos.               
*  `clear`             -   Limpia la pantalla.                         
*  `clearws`           -   Limpia el workspace.                        
*  `tabla`             -   Muestra la tabla de símbolos.               
*  `exit`              -   Sale del programa.                          
*  `help`              -   Muestra la ayuda.   

## Ejecución
Para la compilación de este intérprete matemático se descargará o clonará este repositorio y se insertarán todos los archivos en un mismo directorio. Una vez insertados, se abrirá una terminal Linux, se accederá al directorio con los archivos y se ejecutará el comando `make` para compilarlo.

Una vez compilado, el intérprete matemático se ejecutará por terminal con el comando `./nuriMath`. Si se quiere iniciar ejecutando un fichero de entrada, también se puede iniciar el intérprete ejecutando `./nuriMath [FICHERO.n]`. Por otra parte, si se quiere iniciar mostrando la ayuda del intérprete se puede iniciar ejecutando `./nuriMath -h` , o ejecutando el comando `help` ( o `help()`) una vez iniciado el intérprete.
