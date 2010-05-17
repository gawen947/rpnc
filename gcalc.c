/* File: gcalc.c
   Time-stamp: <2010-05-18 01:51:01 gawen>

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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define VERSION      "0.3-git"
#define PACKAGE      "gcalc"
#define PRECISION    "12"

struct lifo
{
  int index;
  const char **stk;
};

struct operator
{
  const char *name;
  double (*func)(struct lifo *);
};

static double pop(struct lifo * stack);
static void show(double x);
static bool isfloat(const char * str);

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
  printf(PACKAGE " " VERSION "\n");
  exit(stack->index);
  return NAN;
}

static double opr_help(struct lifo * stack)
{
  printf("Usage: " PACKAGE " [OPERATORS]\n");
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
      {"+",opr_add},
      {"-",opr_sub},
      {"*",opr_prd},
      {".",opr_prd},
      {"/",opr_div},
      {"^",opr_pow},
      {"**",opr_pow},
      {"pow",opr_pow},
      {"atan2",opr_atan2},
      {"hypot",opr_hypot},
      {"mod",opr_mod},
      /* unary */
      {"inv",opr_inv},
      {"sqrt",opr_sqrt},
      {"cbrt",opr_cbrt},
      {"sin",opr_sin},
      {"cos",opr_cos},
      {"tan",opr_tan},
      {"asin",opr_asin},
      {"acos",opr_acos},
      {"atan",opr_atan},
      {"sinh",opr_sinh},
      {"cosh",opr_cosh},
      {"tanh",opr_tanh},
      {"asinh",opr_asinh},
      {"acosh",opr_acosh},
      {"atanh",opr_atanh},
      {"exp",opr_exp},
      {"exp2",opr_exp2},
      {"expm1",opr_expm1},
      {"log",opr_log},
      {"log10",opr_log10},
#ifndef __FreeBSD__
      {"log2",opr_log2},
#endif /* __FreeBSD__ */
      {"logb",opr_logb},
      {"log1p",opr_log1p},
      {"abs",opr_abs},
      {"erf",opr_erf},
      {"erfc",opr_erfc},
      {"lgamma",opr_lgamma},
      {"tgamma",opr_tgamma},
      {"=",opr_show},
      /* void */
      {"pi",opr_pi},
      {"c",opr_c},
      {"e",opr_e},
      {"me",opr_me},
      {"mp",opr_mp},
      {"G",opr_G},
      {"g",opr_g},
      {"h",opr_h},
      {"hbar",opr_hbar},
      {"eps0",opr_eps0},
      {"mu0",opr_mu0},
      {"NA",opr_NA},
      {"stdT",opr_stdT},
      {"stdP",opr_stdP},
      {"INF",opr_inf},
      {"+INF",opr_inf},
      {"-INF",opr_ninf},
      {"version",opr_version},
      {"help",opr_help},
      {NULL,NULL}
    };

  if(!stack->index) {
    fprintf(stderr,"Syntax error : empty stack\n");
    exit(stack->index);
  }
  operator = stack->stk[stack->index--];
  for(i = ops ; i->name ; i++)
    if(!strcmp(operator,i->name))
      return i->func(stack);
  if(isfloat(operator))
     return atof(operator);
  fprintf(stderr,"This operator \"%s\" does not exists valids are :\n",
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

int main(int argc, const char *argv[])
{
  struct lifo stack = { --argc, argv };
  show(pop(&stack));
  exit(stack.index);
}
