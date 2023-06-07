%{
#include <math.h>

#include "tablaSimbolos.h"
#include "errores.h"
#include "lex.yy.h"

ComponenteLexico componenteLexico;
int echo = 1; // 0: no echo, 1: echo
int script = 0; // 0: no script, 1: script
int error = 0; // 0: no error, 1: error

void cambiarScript(int valor);
void cambiarEcho(int valor);
%}

%start input

/* Declaraciones de BISON */

/*Declaraciones de tipos*/
%union {
    double numero;     /* Para devolver números */
    char *cadena;   /* Para devolver puntero a char */
}

%token <numero> NUM /* Números */
%token <cadena> VAR FUN CONS FICH COMARG COM LIB /* Variable, Funcion, Constante, Fichero, Comndo con argumento, Comando sin argumento, Librería */

%type <numero> exp asig com fun

/* Precedencia de operadores */
%left '-' '+' 
%left '*' '/'
%left '%'
%precedence NEG
%right '^'

%%

/* Reglas gramaticales */

input: /* vacío */ %empty    { if(!script) printf(AMARILLO">>"PREDETERMINADO" "); }
        | input linea
;

linea:        '\n'           { if(!script) printf(AMARILLO">>"PREDETERMINADO" "); }
        | error '\n'         { yyerrok; if(!script) printf(AMARILLO">>"PREDETERMINADO" ");}  /* En caso de haber error recupera el error para continuar el analisis*/
        | error ';' '\n'     { yyerrok; if(!script) printf(AMARILLO">>"PREDETERMINADO" ");}  /* En caso de haber error recupera el error para continuar el analisis*/
        | exp '\n'           {
                                if(!error){
                                        if(isnan($1)){
                                                imprimirError(DETECTADO_NAN);
                                        }else if(echo) printf (VERDE"\t%lf"PREDETERMINADO"\n\n", $1);
                                        }
                                if(!script) printf(AMARILLO">>"PREDETERMINADO" "); 
                                error = 0;
                             }
        | asig '\n'          { 
                                if(!error){
                                        if(isnan($1)){
                                                imprimirError(DETECTADO_NAN);
                                        }else if(echo) printf (VERDE"\t%lf"PREDETERMINADO"\n\n", $1);
                                        }
                                if(!script) printf(AMARILLO">>"PREDETERMINADO" "); 
                                error = 0;
                             }
        | fun '\n'           { 
                                if(!error){
                                        if(isnan($1)){
                                                imprimirError(DETECTADO_NAN);
                                        }else if(echo) printf (VERDE"\t%lf"PREDETERMINADO"\n\n", $1);
                                        }
                                if(!script) printf(AMARILLO">>"PREDETERMINADO" "); 
                                error = 0;
                             }
        /* Si acaba en ; no se imprime el resultado */
        | exp ';' '\n'       {
                                if(!error && isnan($1)) imprimirError(DETECTADO_NAN);
                                if(!script) printf(AMARILLO">>"PREDETERMINADO" "); 
                                error = 0;
                             }
        | asig ';' '\n'      {
                                if(!error && isnan($1)) imprimirError(DETECTADO_NAN);
                                if(!script) printf(AMARILLO">>"PREDETERMINADO" "); 
                                error = 0;
                             }
        | fun ';' '\n'       {
                                if(!error && isnan($1)) imprimirError(DETECTADO_NAN);
                                if(!script) printf(AMARILLO">>"PREDETERMINADO" "); 
                                error = 0;
                             }
        | com '\n'           {  
                                if(!error && isnan($1)) imprimirError(DETECTADO_NAN);
                                if(!script) printf(AMARILLO">>"PREDETERMINADO" "); 
                                error = 0;
                             }
        | com ';' '\n'       {
                                if(!error && isnan($1)) imprimirError(DETECTADO_NAN);
                                if(!script) printf(AMARILLO">>"PREDETERMINADO" "); 
                                error = 0;
                             }

                             
;

exp:      NUM                
        | VAR                { 
                                componenteLexico = buscarComponenteLexico($1);
                                if (componenteLexico.lexema == NULL) {
                                        imprimirError(VARIABLE_NO_DEFINIDA);
                                        error = 1;
                                        $$ = NAN;
                                }else {
                                        $$ = componenteLexico.valor.numero;
                                }
                                free($1);
                             }
        | CONS               { 
                                componenteLexico = buscarComponenteLexico($1);                       
                                $$ = componenteLexico.valor.numero;
                                free($1);
                             }
        | exp '+' exp        { 
                                if (!isnan($1) && !isnan($3)) {
                                        $$ = $1 + $3;
                                } else {
                                        $$ = NAN;
                                }
                             }
        | exp '-' exp        { 
                                if (!isnan($1) && !isnan($3)) {
                                        $$ = $1 - $3;
                                } else {
                                        $$ = NAN;
                                }
                             }
        | exp '*' exp        { 
                                if (!isnan($1) && !isnan($3)) {
                                        $$ = $1 * $3;
                                } else {
                                        $$ = NAN;
                                }
                             }
        | exp '/' exp        { 
                                if($3 == 0){
                                        imprimirError(DIVISION_CERO);
                                        error = 1;
                                        $$ = NAN;
                                } else {
                                        if (!isnan($1) && !isnan($3)) {
                                                $$ = $1 / $3;
                                        } else {
                                                $$ = NAN;
                                        }
                                }
                             }
        | '-' exp %prec NEG  { 
                                if (!isnan($2)) {
                                        $$ = -$2;
                                } else {
                                        $$ = NAN;
                                }
                             }
        | exp '%' exp        {
                                if ($3 == 0) {
                                        imprimirError(MODULO_CERO);
                                        error = 1;
                                        $$ = NAN;
                                } else {
                                        if (!isnan($1) && !isnan($3)) {
                                                $$ = fmod($1, $3);
                                        } else {
                                                $$ = NAN;
                                        }
                                }
                             }
        | exp '^' exp        { 
                                if (!isnan($1) && !isnan($3)) {
                                        $$ = pow($1, $3);
                                } else {
                                        $$ = NAN;
                                } 
                             }
        | '(' exp ')'        { $$ = $2; }

;

asig:     VAR '=' exp        { 
                                if(!error){
                                        componenteLexico = buscarComponenteLexico($1);
                                        if(componenteLexico.lexema == NULL){
                                                componenteLexico.lexema = strdup($1);
                                                componenteLexico.compLexico = VAR;
                                                componenteLexico.valor.numero = $3;
                                                insertarComponenteLexico(componenteLexico);
                                        }else{
                                                modificarValorVariable($1,$3);
                                        }
                                        $$ = $3; 
                                        free($1);
                                }
                             }
        | VAR '=' fun        {   if(!error){
                                        componenteLexico = buscarComponenteLexico($1);
                                        if(componenteLexico.lexema == NULL){
                                                componenteLexico.lexema = strdup($1);
                                                componenteLexico.compLexico = VAR;
                                                componenteLexico.valor.numero = $3;
                                                insertarComponenteLexico(componenteLexico);
                                        }else{
                                                modificarValorVariable($1,$3);
                                        }
                                        $$ = $3; 
                                        free($1);
                                }
                             }
        | CONS '=' exp       { 
                                imprimirError(ASIGNACION_CONSTANTE); 
                                error = 1; 
                                $$ = NAN;
                                free($1);
                                
                             }
        | CONS '=' fun       { 
                                imprimirError(ASIGNACION_CONSTANTE); 
                                error = 1; 
                                $$ = NAN;
                                free($1);
                                
                             }
;

com:      COM                   { 
                                        componenteLexico = buscarComponenteLexico($1);
                                        free($1);
                                        (*componenteLexico.valor.funptr)();
                                }

        | COM '(' ')'          {            
                                        componenteLexico = buscarComponenteLexico($1);
                                        free($1);
                                        (*componenteLexico.valor.funptr)();
                                }
        | COMARG '(' FICH ')'   { 
                                        componenteLexico = buscarComponenteLexico($1);
                                        (*componenteLexico.valor.funptr)($3);
                                        free($1);
                                        free($3);
                                }
        | COMARG '(' VAR ')'    { 
                                        componenteLexico = buscarComponenteLexico($1);
                                        (*componenteLexico.valor.funptr)($3);
                                        free($1);
                                        free($3);
                                }
        | COMARG VAR            {       
                                        componenteLexico = buscarComponenteLexico($1);
                                        (*componenteLexico.valor.funptr)($2);
                                        free($1);
                                        free($2);
                                }
        | COMARG FICH           { 
                                        componenteLexico = buscarComponenteLexico($1);
                                        (*componenteLexico.valor.funptr)($2);
                                        free($1);
                                        free($2);
                                }
        | COMARG LIB           { 
                                        componenteLexico = buscarComponenteLexico($1);
                                        (*componenteLexico.valor.funptr)($2);
                                        free($1);
                                        free($2);
                                }
        | COMARG '(' ')'        { 
                                        imprimirError(ARGUMENTO_NO_INDICADO);
                                        error = 1;
                                        $$ = NAN;
                                        free($1);
                                }    
        | COMARG                { 
                                        imprimirError(ARGUMENTO_NO_INDICADO);
                                        error = 1;
                                        $$ = NAN;
                                        free($1);
                                }                    
        | COM exp               { 
                                        imprimirError(NO_ADMITE_ARGUMENTO);
                                        error = 1;
                                        $$ = NAN;
                                        free($1);
                                }
        | COM FICH              {
                                        imprimirError(NO_ADMITE_ARGUMENTO);
                                        error = 1;
                                        $$ = NAN;
                                        free($1);
                                        free($2);
                                }
        | COM '(' FICH ')'      {
                                        imprimirError(NO_ADMITE_ARGUMENTO);
                                        error = 1;
                                        $$ = NAN;
                                        free($1);
                                        free($3);
                                }
        
;

fun:      FUN '(' exp ')'    { 
                                componenteLexico = buscarComponenteLexico($1);
                                $$ = (*(componenteLexico.valor.funptr))($3);
                                free($1);
                             }
        | FUN '(' exp ',' exp ')'    { 
                                componenteLexico = buscarComponenteLexico($1);
                                $$ = (*(componenteLexico.valor.funptr))($3,$5);
                                free($1);
                             }
        | VAR '(' exp ')'    { 
                                componenteLexico = buscarComponenteLexico($1);
                                if (componenteLexico.lexema == NULL) {
                                        // Buscamos en las librerias importadas
                                        componenteLexico = buscarFuncionLibrerias($1);
                                        if(componenteLexico.compLexico == 1){
                                                imprimirError(FUNCION_VARIAS_LIBRERIAS);
                                                error = 1;
                                                $$ = NAN;
                                        }else if(componenteLexico.lexema == NULL){
                                                imprimirError(VARIABLE_NO_DEFINIDA);
                                                error = 1;
                                                $$ = NAN;
                                        }else{
                                               $$ = (*(componenteLexico.valor.funptr))($3);
                                        }
                                }else {
                                        $$ = (*(componenteLexico.valor.funptr))($3);
                                }
                                free($1);
                             }
        | VAR '(' exp ',' exp ')'    { 
                                componenteLexico = buscarComponenteLexico($1);
                                if (componenteLexico.lexema == NULL) {
                                        // Buscamos en las librerias importadas
                                        componenteLexico = buscarFuncionLibrerias($1);
                                        if(componenteLexico.compLexico == 1){
                                                imprimirError(FUNCION_VARIAS_LIBRERIAS);
                                                error = 1;
                                                $$ = NAN;
                                        }else if(componenteLexico.lexema == NULL){
                                                imprimirError(VARIABLE_NO_DEFINIDA);
                                                error = 1;
                                                $$ = NAN;
                                        }else{
                                               $$ = (*(componenteLexico.valor.funptr))($3,$5);
                                        }
                                }else {
                                        $$ = (*(componenteLexico.valor.funptr))($3,$5);
                                }
                                free($1);
                             }
        | LIB '/' VAR '(' exp ')' {
                                        componenteLexico = buscarComponenteLexico($1);
                                        if (componenteLexico.lexema == NULL) {
                                                imprimirError(LIBRERIA_NO_IMPORTADA);
                                                error = 1;
                                                $$ = NAN;
                                        }else {
                                                componenteLexico = buscarFuncionLibreria(componenteLexico, $3);
                                                if(componenteLexico.lexema == NULL){
                                                        imprimirError(OBTENCION_FUNCION);
                                                        error = 1;
                                                        $$ = NAN;
                                                }else{
                                                        $$ = (*(componenteLexico.valor.funptr))($5);
                                                }
                                        }
                                        free($1);
                                        free($3);
                                  }
        | LIB '/' VAR '(' exp ',' exp ')' {
                                        componenteLexico = buscarComponenteLexico($1);
                                        if (componenteLexico.lexema == NULL) {
                                                imprimirError(LIBRERIA_NO_IMPORTADA);
                                                error = 1;
                                                $$ = NAN;
                                        }else {
                                                componenteLexico = buscarFuncionLibreria(componenteLexico, $3);
                                                if(componenteLexico.lexema == NULL){
                                                        imprimirError(OBTENCION_FUNCION);
                                                        error = 1;
                                                        $$ = NAN;
                                                }else{
                                                        $$ = (*(componenteLexico.valor.funptr))($5,$7);
                                                }
                                        }
                                        free($1);
                                        free($3);
                                  }
        | LIB '/' FUN '(' exp ')' {
                                        componenteLexico = buscarComponenteLexico($1);
                                        if (componenteLexico.lexema == NULL) {
                                                imprimirError(LIBRERIA_NO_IMPORTADA);
                                                error = 1;
                                                $$ = NAN;
                                        }else {
                                                componenteLexico = buscarFuncionLibreria(componenteLexico, $3);
                                                if(componenteLexico.lexema == NULL){
                                                        imprimirError(OBTENCION_FUNCION);
                                                        error = 1;
                                                        $$ = NAN;
                                                }else{
                                                        $$ = (*(componenteLexico.valor.funptr))($5);
                                                }
                                        }
                                        free($1);
                                        free($3);
                                  }
        | LIB '/' FUN '(' exp ',' exp ')' {
                                        componenteLexico = buscarComponenteLexico($1);
                                        if (componenteLexico.lexema == NULL) {
                                                imprimirError(LIBRERIA_NO_IMPORTADA);
                                                error = 1;
                                                $$ = NAN;
                                        }else {
                                                componenteLexico = buscarFuncionLibreria(componenteLexico, $3);
                                                if(componenteLexico.lexema == NULL){
                                                        imprimirError(OBTENCION_FUNCION);
                                                        error = 1;
                                                        $$ = NAN;
                                                }else{
                                                        $$ = (*(componenteLexico.valor.funptr))($5,$7);
                                                }
                                        }
                                        free($1);
                                        free($3);
                                  }
;

/* Fin de la gramática */
%%

/* Código adicional */

void cambiarScript(int valor){
        script = valor;
        if(script){
                printf("\n"VERDE"\tScript ejecutado correctamente."PREDETERMINADO"\n\n");
        }else{
                printf("\n"AMARILLO">>"PREDETERMINADO" ");

        }
}

void cambiarEcho(int valor){
        echo = valor;
        if (echo) {
        printf(VERDE"\tEcho activado."PREDETERMINADO"\n\n");
        } else {
                printf(ROJO"\tEcho desactivado."PREDETERMINADO"\n\n");
        }
}