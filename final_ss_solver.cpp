#include <TXLib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>



typedef struct SS_Tests
{
    int    num_Test;
    double        a;
    double        b;
    double        c;
    double   x1_exp;
    double   x2_exp;
    int n_roots_exp;
} SS_Tests;

int SS_Solver_Func(double a, double b, double c, double* x1, double* x2);
int Quadric_Func(double a, double b, double c, double* x1, double* x2);
int Linear_Func(double b, double c, double* x1, double* x2);
int  sollution(double x1, double x2, int n_roots);
void Input_Func(double* a, double* b, double* c);
int compare_roots(double x1, double x1_exp);
int Double_Diff_comp(double a, double b);
void func_Tester(SS_Tests data);
void Test_func();

//--------Enum-for-roots-cases-----------------------------------------------------------------------------

typedef enum Roots_case
{
    ZERO_Roots =  0,
    ONE_Root =    1,
    TWO_Roots =   2,
    INF_Roots =  -1
} Roots_case;

//---------------------------------MAIN--------------------------------------------------------------------

int main (void)
{
    Test_func();

    double a = 0;
    double b = 0;
    double c = 0;

    Input_Func(&a, &b, &c);

    double   x1 = 0;
    double   x2 = 0;
    int n_roots = SS_Solver_Func(a, b, c, &x1, &x2);

    sollution(x1, x2, n_roots);
}

//---Introduction-of-a-constant-and-a-function-comparing-the-difference-of-double-numbers--------------



int Double_Diff_comp(double a, double b)
{
    const double Const_E = 0.0000000001;
    double Diff = a - b;
    if (fabs(Diff) < Const_E)  return 1;
    else                       return 0;
}

//----------------Square-Equation-Solving-Function-----------------------------------------------------
/*linear equation (a==0)*/

int Linear_Func(double b, double c, double* x1, double* x2)
{
    assert(x1 !=  NULL);
    assert(x2 !=  NULL);
    assert(isfinite(b));
    assert(isfinite(c));

    if (Double_Diff_comp(b,0))
    {
        if (Double_Diff_comp(c,0))
        {
            *x1 = NAN;
            *x2 = NAN;
            return INF_Roots;
        }
        else
        {
            *x1 = NAN;
            *x2 = NAN;
            return ZERO_Roots;
        }
    }
    else
    {
        if (Double_Diff_comp(c,0))
        {
            *x2 = NAN;
            return ONE_Root;
        }
        else
        {
            *x1 = (-c/b);
            *x2 = NAN;
            return ONE_Root;
        }
    }
}

/*quadric equation (a!=0)*/

int Quadric_Func(double a, double b, double c, double* x1, double* x2)
{
    assert(x1 !=  NULL);
    assert(x2 !=  NULL);
    assert(isfinite(a));
    assert(isfinite(b));
    assert(isfinite(c));

    {
        double SS_Discr = ((b*b)) - (4*a*c);
        if (SS_Discr > 0)
        {
            double sqrt_Discr = sqrt(SS_Discr);
            *x1 = ((-b) + sqrt_Discr)/(2*a);
            *x2 = ((-b) - sqrt_Discr)/(2*a);
            return TWO_Roots;
        }
        else if (Double_Diff_comp(SS_Discr,0))
        {
            *x1 = (-b)/(2*a);
            *x2 = NAN;
            return ONE_Root;
        }
        else
        {
            *x1 = NAN;
            *x2 = NAN;
            return ZERO_Roots;
        }
    }
 }

/*����� ������� ��� ��� ��������� ���������, ��� � ��� �������������*/

int SS_Solver_Func(double a, double b, double c, double* x1, double* x2)
{
    int n_roots = 0;

    if (Double_Diff_comp(a, 0))
    {
        n_roots = Linear_Func(b, c, x1, x2);
    }
    else
    {
        n_roots = Quadric_Func(a, b, c, x1, x2);
    }

    return n_roots;
}

//-------Function-for-Testing--------------------------------------int  readable_coef(double x1, double x2, int n_roots)----------------------------------------

void func_Tester(SS_Tests data)
{
    double x1 = 0;
    double x2 = 0;

    int n_roots = SS_Solver_Func(data.a, data.b, data.c, &x1, &x2);

    if     ((n_roots !=  data.n_roots_exp)
        || (compare_roots(x1, data.x1_exp)==0)
        || (compare_roots(x2, data.x2_exp)==0))
    {
        printf("# ErrorTest_%d! Compiled: a = %lg, b = %lg, c = %lg, x1 = %lg, x2 = %lg, n_roots = %int.\n"
               "# Expected: x1 = %lg, x2 = %lg, n_roots = %int.\n",
                data.num_Test,
                data.a,
                data.b,
                data.c,
                x1,
                x2,
                n_roots,
                data.x1_exp,
                data.x2_exp,
                data.n_roots_exp);
    }
}

//----------------------------------------------------------------------------------------------------------

int compare_roots(double x1, double x1_exp)
{
    if ((Double_Diff_comp(x1, x1_exp))||(isnan(x1) && isnan(x1_exp)))
    {
         return 1;
    }
    else return 0;
}

//-----------------------------------------------------------------------------------------------------------

int  sollution(double x1, double x2, int n_roots)
{
    switch (n_roots)
    {
        case ZERO_Roots: return printf ("No roots found.\n");
        break;

        case   ONE_Root: return printf ("Given Square Equation has only 1 (one) root: x = %lg\n", x1);
        break;

        case  TWO_Roots: return printf ("Given Square Equation has 2 (two) roots: x1 = %lg, x2 = %lg\n", x1, x2);
        break;

        case  INF_Roots: return printf ("Given Square Equation has Infinite number of roots.");
        break;

        default:         return printf ("main(): ERROR: n_roots = %d\n", n_roots);
        break;
    }
}

//-----------------------------------------------------------------------------------------------------------

void Test_func()
{
struct SS_Tests mass[] =    {{.num_Test = 0,   .a =    0,  .b =   5,  .c =  8,  .x1_exp =    -1.6,  .x2_exp =      NAN,    .n_roots_exp =  1},
                             {.num_Test =  1,  .a =    0,  .b =   0,  .c =  0,  .x1_exp =     NAN,  .x2_exp =      NAN,    .n_roots_exp = -1},
                             {.num_Test =  2,  .a =    0,  .b =   0,  .c =  8,  .x1_exp =     NAN,  .x2_exp =      NAN,    .n_roots_exp =  0},
                             {.num_Test =  3,  .a =    0,  .b =   5,  .c =  0,  .x1_exp =       0,  .x2_exp =      NAN,    .n_roots_exp =  1},
                             {.num_Test =  4,  .a =    1,  .b =   0,  .c =  0,  .x1_exp =       0,  .x2_exp =      NAN,    .n_roots_exp =  1},
                             {.num_Test =  5,  .a =    7,  .b =   0,  .c =  8,  .x1_exp =     NAN,  .x2_exp =      NAN,    .n_roots_exp =  0},
                             {.num_Test =  6,  .a =    4,  .b =   0,  .c = -8,  .x1_exp = sqrt(2),  .x2_exp = -sqrt(2),    .n_roots_exp =  2},
                             {.num_Test =  7,  .a =    1,  .b = -12,  .c = 36,  .x1_exp =       6,  .x2_exp =      NAN,    .n_roots_exp =  1},
                             {.num_Test =  8,  .a = -224,  .b =   5,  .c = -8,  .x1_exp =     NAN,  .x2_exp =      NAN,    .n_roots_exp =  0},
                             {.num_Test =  9,  .a = -486,  .b =   5,  .c = -8,  .x1_exp =     NAN,  .x2_exp =      NAN,    .n_roots_exp =  0},
                             {.num_Test = 10,  .a =    1,  .b =  -4,  .c =  5,  .x1_exp =     NAN,  .x2_exp =      NAN,    .n_roots_exp =  0}
                            };

    int nTests = (sizeof(mass)/sizeof(mass[0]));

    for(int i=0; i < nTests; i++)
    {
        func_Tester(mass[i]);
    }
}

//------------------------------------------------------------------------------------------------------------

void Input_Func(double* a, double* b, double* c)
{
    printf ("# Enter coefficients of a square equation a, b, c:   \n");
    printf ("# Please use real numbers instead of letters, words etc.  \n");
    printf ("# If you would love the program to stop running, please enter 'Thanks!' \n");

    while (scanf("%lg %lg %lg", a, b, c) != 3)
    {
        while (getchar() != '\n') {}
        printf ("#Invalid input! Please, enter numbers only! =(  \n");
    }
}
