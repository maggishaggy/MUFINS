/* 
 * File:   Text.h
 * Author: t00383
 *
 * Created on 18 January 2010, 13:58
 */

#ifndef _TEXT_H
#define	_TEXT_H



#include <cstring>
#include <sstream>
#include <algorithm>
#include <cfloat>
#include <cmath>

#include "Types.h"


#define NEWLINE     '\n'
#define TAB         '\t'
#define EQUAL       '='
#define PLUS        '+'
#define SPACE       ' '
#define EMPTYSTR    ""
#define COMMENT     '#'
#define COMMA       ','
#define COLON       ':'

#define ENDOFRECORD ';'
#define CONTINUE    "@"

#define OR          "OR"
#define AND         "AND"
#define OPEN_PAR    "("
#define CLOS_PAR    ")"

#define OPENSET     "{"
#define CLOSESET    "}"
#define COMMASPACE  ", "
#define TAB1        "\t"
#define TAB2        "\t\t"
#define LISTDELIM   " "
#define DELIM       TAB

#define CONT GLP_CV  //w/continuous
#define INT GLP_BV //w/binary, since we'll hardly need other integer variables
#define ALLKIND   0 //w/ all kinds of variables


//split a string into substrings using a delimiter
//strvec split(string s, const char* delim = DELIM);
void split(string* s, strvec* vec, char delim = DELIM);
void split(istream &is, strvec* vec, char delim = DELIM);
void split2(string* s, strvec* vec, char delim = DELIM);//w/
void split2(istream &is, strvec* vec, char delim = DELIM);//w/
void remove_chars(string *s, const char* chars = " ");

string num2str(double d);

ostream &operator << (ostream &out, const RANGE &range);

ostream &operator << (ostream &out, const stomap &sto);
ostream &operator << (ostream &out, const strvec &vec);

ostream &operator << (ostream &out, const POINT &point);
ostream &operator << (ostream &out, const pointvec &pv);


ostream &operator << (ostream &out, const POINT &point);
ostream &operator << (ostream &out, const pointvec &pv);



//stomap operator * (stomap &sto, double coef);
void mul(stomap *sto, double coef);

double str2double(string s);
double str2int(string s);//w/
bool is_infinite(double d);

bool isallspace(string s);

void set_sto(string name, double coef, stomap* sto);

//string replaced(string s, char c1, char c2);

bool is_operator(string s);

#endif	/* _TEXT_H */

