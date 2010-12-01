/* File: gcalc.c
   Time-stamp: <2010-12-01 19:38:46 gawen>

   Copyright (C) 2010 David Hauweele <david.hauweele@gmail.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <sysexits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>
#include <fenv.h>
#include <err.h>

#define VERSION      "0.3"
#define PACKAGE      "gcalc"
#ifndef COMMIT
# define PACKAGE_VERSION VERSION
#else
# define PACKAGE_VERSION VERSION " (commit:" COMMIT ")"
#endif /* COMMIT */

#define PRECISION    "12"

#ifdef ARG_MAX
# define STACK_LIMIT ARG_MAX
#else
# define STACK_LIMIT 4096
#endif

#ifndef M_PI
# define M_PI 3.141592653589793238462643
#endif /* M_PI */

struct lifo
{
  int index;
  const char **stk;
};

struct operator
{
  const char *name;
  const char *help;
  double (*func)(struct lifo *);
};

static double pop(struct lifo * stack);
static void show(double x);
static bool isfloat(const char * str);
static void operators(const struct operator * ops);

/* BEGINNING of operator definition*/
static double opr_add(struct lifo * stack)
{
  double op1,op2;
  op1 = pop(stack);
  op2 = pop(stack);
  return op2 + op1;
}

static double opr_sub(struct lifo * stack)
{
  double op1,op2;
  op1 = pop(stack);
  op2 = pop(stack);
  return op2 - op1;
}

static double opr_prd(struct lifo * stack)
{
  double op1,op2;
  op1 = pop(stack);
  op2 = pop(stack);
  return op2 * op1;
}

static double opr_div(struct lifo * stack)
{
  double op1,op2;
  op1 = pop(stack);
  op2 = pop(stack);
  return op2 / op1;
}

static double opr_pow(struct lifo * stack)
{
  double op1,op2;
  op1 = pop(stack);
  op2 = pop(stack);
  return pow(op2,op1);
}

static double opr_mod(struct lifo * stack)
{
  double op1,op2;
  op1 = pop(stack);
  op2 = pop(stack);
  return fmod(op2,op1);
}

static double opr_hypot(struct lifo * stack)
{
  double op1,op2;
  op1 = pop(stack);
  op2 = pop(stack);
  return hypot(op2,op1);
}

static double opr_atan2(struct lifo * stack)
{
  double op1,op2;
  op1 = pop(stack);
  op2 = pop(stack);
  return atan2(op2,op1);
}

static double opr_inv(struct lifo * stack)
{
  double op = pop(stack);
  return 1 / op;
}

static double opr_sqrt(struct lifo * stack)
{
  double op = pop(stack);
  return sqrt(op);
}

static double opr_cbrt(struct lifo * stack)
{
  double op = pop(stack);
  return cbrt(op);
}

static double opr_sin(struct lifo * stack)
{
  double op = pop(stack);
  return sin(op);
}

static double opr_cos(struct lifo * stack)
{
  double op = pop(stack);
  return cos(op);
}

static double opr_tan(struct lifo * stack)
{
  double op = pop(stack);
  return tan(op);
}

static double opr_asin(struct lifo * stack)
{
  double op = pop(stack);
  return asin(op);
}

static double opr_acos(struct lifo * stack)
{
  double op = pop(stack);
  return acos(op);
}
static double opr_atan(struct lifo * stack)
{
  double op = pop(stack);
  return atan(op);
}

static double opr_sinh(struct lifo * stack)
{
  double op = pop(stack);
  return sinh(op);
}

static double opr_cosh(struct lifo * stack)
{
  double op = pop(stack);
  return cosh(op);
}

static double opr_tanh(struct lifo * stack)
{
  double op = pop(stack);
  return tanh(op);
}

static double opr_asinh(struct lifo * stack)
{
  double op = pop(stack);
  return asinh(op);
}

static double opr_acosh(struct lifo * stack)
{
  double op = pop(stack);
  return acosh(op);
}

static double opr_atanh(struct lifo * stack)
{
  double op = pop(stack);
  return atanh(op);
}

static double opr_exp(struct lifo * stack)
{
  double op = pop(stack);
  return exp(op);
}

static double opr_exp2(struct lifo * stack)
{
  double op = pop(stack);
  return exp2(op);
}

static double opr_expm1(struct lifo * stack)
{
  double op = pop(stack);
  return expm1(op);
}

static double opr_log(struct lifo * stack)
{
  double op = pop(stack);
  return log(op);
}

static double opr_log10(struct lifo * stack)
{
  double op = pop(stack);
  return log10(op);
}

#ifndef __FreeBSD__
static double opr_log2(struct lifo * stack)
{
  double op = pop(stack);
  return log2(op);
}
#endif /* __FreeBSD__ */

static double opr_logb(struct lifo * stack)
{
  double op = pop(stack);
  return logb(op);
}

static double opr_log1p(struct lifo * stack)
{
  double op = pop(stack);
  return log1p(op);
}

static double opr_abs(struct lifo * stack)
{
  double op = pop(stack);
  return fabs(op);
}

static double opr_erf(struct lifo * stack)
{
  double op = pop(stack);
  return erf(op);
}

static double opr_erfc(struct lifo * stack)
{
  double op = pop(stack);
  return erfc(op);
}

static double opr_lgamma(struct lifo * stack)
{
  double op = pop(stack);
  return lgamma(op);
}

static double opr_tgamma(struct lifo * stack)
{
  double op = pop(stack);
  return tgamma(op);
}

static double opr_show(struct lifo * stack)
{
  double op = pop(stack);
  show(op);
  return op;
}

static double opr_pi(struct lifo * stack)
{
  return M_PI;
}

static double opr_c(struct lifo * stack)
{
  return 299792458;
}

static double opr_e(struct lifo * stack)
{
  return 1.60217733E-19;
}

static double opr_me(struct lifo * stack)
{
  return 9.1093826E-31;
}

static double opr_mp(struct lifo * stack)
{
  return 1.6726231E-17;
}

static double opr_G(struct lifo * stack)
{
  return 6.67259E-11;
}

static double opr_g(struct lifo * stack)
{
  return 9.80665;
}

static double opr_h(struct lifo * stack)
{
  return 6.6260755E-34;
}

static double opr_hbar(struct lifo * stack)
{
  return 1.0545726691251E-34;
}

static double opr_eps0(struct lifo * stack)
{
  return 8.85418781761E-12;
}

static double opr_mu0(struct lifo * stack)
{
  return 1.25663706144E-06;
}

static double opr_NA(struct lifo * stack)
{
  return 6.0221367E23;
}

static double opr_stdT(struct lifo * stack)
{
  return 273.15;
}

static double opr_stdP(struct lifo * stack)
{
  return 101325;
}

static double opr_inf(struct lifo *stack)
{
  return INFINITY;
}

static double opr_ninf(struct lifo *stack)
{
  return -INFINITY;
}

static double opr_version(struct lifo * stack)
{
  printf(PACKAGE " " PACKAGE_VERSION "\n");
  exit(stack->index);
  return NAN;
}

static double opr_help(struct lifo * stack)
{
  printf("Usage: " PACKAGE " [operators]\n");
  exit(stack->index);
  return NAN;
}
/* END of operator definition */

static double pop(struct lifo *stack)
{
  const char *operator;
  const struct operator *i;
  const struct operator ops[] =
    {
      /* binary */
      {"+","Addition",opr_add},
      {"-","Substraction",opr_sub},
      {"*","Multiplication",opr_prd},
      {".","Multiplication",opr_prd},
      {"/","Division",opr_div},
      {"^","Raise to a power",opr_pow},
      {"**","Raise to a power",opr_pow},
      {"pow","Raise to a power",opr_pow},
      {"atan2","Inverse tangent of a quotient",opr_atan2},
      {"hypot","Hypotenuse by the Pythagorean formula",opr_hypot},
      {"mod","Modulo operation",opr_mod},
      /* unary */
      {"inv","Multiplicative inverse",opr_inv},
      {"sqrt","Square root",opr_sqrt},
      {"cbrt","Cube root",opr_cbrt},
      {"sin","Sinus of a radian angle",opr_sin},
      {"cos","Cosine of a radian angle",opr_cos},
      {"tan","Tangent of a radian angle",opr_tan},
      {"asin","Inverse sine in radians",opr_asin},
      {"acos","Inverse cosine in radians",opr_acos},
      {"atan","Inverse tangent in radians",opr_atan},
      {"sinh","Hyperbolic sine",opr_sinh},
      {"cosh","Hyperbolic cosine",opr_cosh},
      {"tanh","Hyperbolic tangent",opr_tanh},
      {"asinh","Inverse hyperbolic sine",opr_asinh},
      {"acosh","Inverse hyperbolic cosine",opr_acosh},
      {"atanh","Inverse hyperbolic tangent",opr_atanh},
      {"exp","Natural exponential",opr_exp},
      {"exp2","Base two exponential",opr_exp2},
      {"expm1","Natural exponential minus one",opr_expm1},
      {"log","Natural logarithm",opr_log},
      {"log10","Base-10 logarithm",opr_log10},
#ifndef __FreeBSD__
      {"log2","Base two logarithm",opr_log2},
#endif /* __FreeBSD__ */
      {"logb","Obtains the exponent",opr_logb},
      {"log1p","Add the natural logarithm of one",opr_log1p},
      {"abs","Absolute value",opr_abs},
      {"erf","Error function",opr_erf},
      {"erfc","Complementary error function",opr_erfc},
      {"lgamma","Natural logarithm of the absolute value of the gamma function",
       opr_lgamma},
      {"tgamma","True gamma function",opr_tgamma},
      {"=","Show intermediate results",opr_show},
      /* void */
      {"pi","Pi constant",opr_pi},
      {"c","Speed of light (m/s)",opr_c},
      {"e","Electron charge (C)",opr_e},
      {"me","Electron mass (kg)",opr_me},
      {"mp","Proton mass (kg)",opr_mp},
      {"G","Gravitational constant (N.m^2/kg^2)",opr_G},
      {"g","Standard gravity (m/s^2)",opr_g},
      {"h","Planck constant (J.s)",opr_h},
      {"hbar","Dirac constant (J.s)",opr_hbar},
      {"eps0","Vacuum permittivity (F/m)",opr_eps0},
      {"mu0","Vacuum permeability (H/m)",opr_mu0},
      {"NA","Avogadro constant (mol^-1)",opr_NA},
      {"stdT","Standard temperature (K)",opr_stdT},
      {"stdP","Standard pression (Pa)",opr_stdP},
      {"INF","Infinity",opr_inf},
      {"+INF","Infinity",opr_inf},
      {"-INF","Minus infinity",opr_ninf},
      {"version","Display version",opr_version},
      {"help","Display usage",opr_help},
      {"operators","Display operators",NULL},
      {NULL,NULL,NULL}
    };

  if(!stack->index)
    errx(stack->index, "syntax error: Empty stack");
  operator = stack->stk[stack->index--];
  if(!strcmp(operator,"operators")) {
    operators(ops);
    exit(stack->index);
  }
  for(i = ops ; i->name ; i++) {
    if(!strcmp(operator,i->name)) {
      int raised;
      double res;

      errno = 0;
      feclearexcept(FE_ALL_EXCEPT);

      res = i->func(stack);

      raised = fetestexcept(FE_INVALID | FE_DIVBYZERO |
                            FE_OVERFLOW | FE_UNDERFLOW);
      if(raised || errno) {
        const char *msg = NULL;
        if(errno)
          err(stack->index, "math error with ‘%s’", operator);
        else if(raised & FE_INVALID)
          msg = "Invalid operation";
        else if(raised & FE_DIVBYZERO)
          msg = "Division by zero";
        else if(raised & FE_OVERFLOW)
          msg = "Result not representable due to underflow";
        else if(raised & FE_UNDERFLOW)
          msg = "Result not representable due to overflow";
        errx(stack->index, "math error with ‘%s’: %s", operator, msg);
      }

      return res;
    }
  }
  if(isfloat(operator))
     return atof(operator);
  fprintf(stderr,"This operator ‘%s’ does not exists valids are:\n",
          operator);
  for(i = ops ; i->name ; i++)
    fprintf(stderr,"  %s",i->name);
  fprintf(stderr,"\n");
  exit(stack->index);
  return 0.;
}

static void show(double x)
{
  printf("%." PRECISION "g\n",x);
}

static bool isfloat(const char * str)
{
  unsigned int dots  = 0;
  if(*str == '+' || *str == '-')
    str++;
  while(*str) {
    if(*str == '.')
      dots++;
    /* TODO: fix E/e position */
    else if(!isdigit(*str) && *str != 'E' && *str != 'e')
      return false;
    str++;
  }
  if(dots > 1)
    return false;
  return true;
}

static void operators(const struct operator * ops)
{
  int max,size;
  const struct operator *i;

  max = 0;
  for(i = ops ; i->name ; i++) {
    size = strlen(i->name);
    if(size > max)
      max = size;
  }
  for(i = ops ; i->name ; i++) {
    size = strlen(i->name);
    printf("%s",i->name);
    for(; size < max ; size++)
      printf(" ");
    printf(" %s\n",i->help);
  }
}

int main(int argc, const char *argv[])
{
  if(argc > STACK_LIMIT)
    errx(EX_SOFTWARE,"stack is too large");
  struct lifo stack = { --argc, argv };
  show(pop(&stack));
  exit(stack.index);
}
