%{
#include <string.h>
#include <math.h>

#include "nuriMath.h"
#include "bison.tab.h"
#include "tablaSimbolos.h"
#include "errores.h"

// Componente léxico actual
ComponenteLexico compLex = {0, NULL};

//Contador de comentarios anidados
int comentariosAnidados = 0;
%}

/*Condición de arranque para detectar comentarios anidados*/
%x NestingComment

/*Opciones:
* Crear un fichero.h con el nombre indicado.
* No se crearán las funciones input ni yyunput (para evitar warnings de compilación)
* Solo se lee un fichero de entrada.
* Se llevará una cuenta de la línea actual del código que se está leyendo.
*/
%option header-file="lex.yy.h" noinput nounput noyywrap yylineno

/*NUMEROS*/

/* DEFINICIONES */

BinaryDigit [01]
DecimalDigit [0-9]
NonZeroDigit [1-9]
Letter [a-zA-Z]

/*INTEGER LITERAL*/

/*DecimalInteger*/

DecimalDigitUS {DecimalDigit}|_
DecimalDigitsUS {DecimalDigitUS}+
DecimalInteger 0|{NonZeroDigit}{DecimalDigitsUS}?

/*BinaryInteger*/

BinaryDigitUS {BinaryDigit}|_
BinaryDigitsUS {BinaryDigitUS}+
BinaryDigitsNoSingleUS {BinaryDigit}{BinaryDigitsUS}?|{BinaryDigitsUS}({BinaryDigit}{BinaryDigitUS}?)?
BinPrefix 0b|0B
BinaryInteger {BinPrefix}{BinaryDigitsNoSingleUS}

INTEGER_LITERAL {DecimalInteger}|{BinaryInteger}

/*FLOAT LITERAL*/

/* DecimalFloat*/

DecimalDigitsNoStartingUS {DecimalDigit}{DecimalDigitsUS}?
DecimalDigitsNoSingleUS {DecimalDigit}{DecimalDigitsUS}?|{DecimalDigitsUS}{DecimalDigit}
LeadingDecimal {DecimalInteger}|0{DecimalDigitsNoSingleUS}
DecimalExponentStart [eE][+-]?
DecimalExponent {DecimalExponentStart}{DecimalDigitsNoSingleUS}
DecimalFloat {LeadingDecimal}\.({DecimalDigitsNoStartingUS}{DecimalExponent}?)?|\.{DecimalDigitsNoStartingUS}{DecimalExponent}?|{LeadingDecimal}{DecimalExponent}

FLOAT_LITERAL {DecimalFloat}

NUMERO {INTEGER_LITERAL}|{FLOAT_LITERAL}

/*IDENTIFICADORES*/

IdentifierStart {Letter}|_
IdentifierChar {IdentifierStart}|0|{NonZeroDigit}
IdentifierChars {IdentifierChar}+
IDENTIFIER {IdentifierStart}{IdentifierChars}?

/*OPERADORES*/

OPERADOR_INDIVIDUAL [\+\-\*\/^=\(\),;\r\n]

/*COMENTARIOS*/

LineComment \/\/[^\n]*
BlockComment \/\*([^*]*|\*[^/])*\*\/
COMMENT {BlockComment}|{LineComment}

/* FICHEROS */

/*FICHERO ({Letter}|{DecimalDigit})+(\.({Letter}|{DecimalDigit})+)+|(\/({Letter}|{DecimalDigit})*)*\/({Letter}|{DecimalDigit})+(\.({Letter}|{DecimalDigit})+)+*/

FICHERO ({Letter}|{DecimalDigit})+(\.({Letter}|{DecimalDigit})+)*\.n|(\/({Letter}|{DecimalDigit})+)*(\.({Letter}|{DecimalDigit})+)*\.n

/*LIBRERIAS*/

LIBRERIA ({Letter}|{DecimalDigit})+(\.({Letter}|{DecimalDigit})+)*\.so(\.{DecimalDigit})*|(\/({Letter}|{DecimalDigit})+)*(\.({Letter}|{DecimalDigit})+)*\.so(\.{DecimalDigit})*

/*CARACTERES A IGNORAR*/

IGNORADO [ \r]

%%
	/* REGLAS */

{IGNORADO}

{COMMENT}

"/+"	{BEGIN(NestingComment); comentariosAnidados++;}
<NestingComment>"+/"	{ comentariosAnidados--; if (comentariosAnidados == 0) BEGIN(INITIAL);}
<NestingComment>"/+"	{ comentariosAnidados++;}
<NestingComment>(.|\n)

{OPERADOR_INDIVIDUAL}		{return (int) *yytext;}
{NUMERO}		            {yylval.numero = atof(yytext); return NUM;}
{IDENTIFIER}			    {
                                yylval.cadena = strdup(yytext);
                                compLex = buscarComponenteLexico(yylval.cadena);
                                if(compLex.lexema == NULL) return VAR;
                                return compLex.compLexico;
                            }
{FICHERO}                   {
                                yylval.cadena = strdup(yytext);
                                return FICH;
                            }
{LIBRERIA}                  {
                                yylval.cadena = strdup(yytext);
                                return LIB;
                            }
.				            {imprimirError(LEXEMA_DESCONOCIDO); yyrestart(stdin); return (int) '\n';}
<<EOF>>		                {cambiarScript(0); fclose(yyin); yyrestart(stdin);}
%%