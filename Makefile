# Opciones de compilación: muestra todos os warnings (-Wall)
CC=gcc -Wall

# Inclusión de las librerías estándares
LIBS = -lm -ldl

# Carpeta de las cabeceras
HEADER_FILES_DIR = .

# Opciones de compilación: indica donde están los ficheros .h
INCLUDES = -I $(HEADER_FILES_DIR)

# Nombre del ejecutable
OUTPUT = nuriMath

# Nombre del fichero de flex
FLEX_FILE = lex.l

# Nombre del fichero de bison
BISON_FILE = bison.y

# Nombre del archivo comprimido
ZIP_FILE = InterpreteMatematico_NuriaGomez.zip

# Ficheros .h
LIB_HEADERS = nuriMath.h lex.yy.h bison.tab.h comands.h tablaSimbolos.h abb.h errores.h

# Ficheros .c
SRCS = main.c nuriMath.c lex.yy.c bison.tab.c comands.c tablaSimbolos.c abb.c errores.c

# Ficheros .o: todos los .o con un análogo .c en SRCS
OBJS = $(SRCS:.c=.o)


# REGLA 1: genera el ejecutable, dependencia de los .o
# Tras generarlos borra los .o
$(OUTPUT): $(OBJS)
	$(CC) -o $(OUTPUT) $(OBJS) $(LIBS)
	rm *.o

# REGLA 2: genera los .o cuando es necesario, dependencia de los .c y .h
# $@ es el nombre del fichero que se genera con la regla (.o)
# $< es el nombre del primer prerrequisito (el archivo .c cuyo .o se está generando)
%.o: %.c $(LIB_HEADERS)
	$(CC) -c -o $@ $< $(INCLUDES)

# REGLA 3: genera el código correspondiente al fichero de flex
flex:
	flex $(FLEX_FILE)
#cp lex.yy_copia.h lex.yy.h
	
# REGRA 4: genera el código correspondiente al fichero de bison
bison:
	bison -Wall -H $(BISON_FILE)
	cp bison.tab_copia.h bison.tab.h
#cp bison.tab_copia.h bison.tab.h

# REGRA 5: genera un archivo comprimido ZIP a partir del código fuente
zip:
	zip $(ZIP_FILE) Makefile README.md bison.tab_copia.h $(SRCS) $(LIB_HEADERS) $(FLEX_FILE) $(BISON_FILE)	
# REGLA 6: borra el ejecutable y el zip
clean:
	rm -f $(OUTPUT)
