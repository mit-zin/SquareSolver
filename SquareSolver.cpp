#include <TXLib.h>
#include <stdio.h>
#include <math.h>

int solver(double a, double b, double c, double* x1, double* x2);
int SquareSolver(double a, double b, double c, double* x1, double* x2);
int NotSquareSolver(double b, double c, double* x);
void entering(double* a, double* b, double* c);
void ans(int rootsn, double x1, double x2);
double MinusNullCheck(double num);
int tester(void);
int nTester(int nTest, double a, double b, double c, double x1Right, double x2Right, int rootsn);

const int SS_ENF = -1;
const double EPSILON = 0.000001;

int main(void)
{
    int nErrors = tester();
    printf("%d\n", nErrors);

    double a = NAN, b = NAN, c = NAN;

    entering(&a, &b, &c);

    double x1 = NAN, x2 = NAN;
    int rootsn = 0;

    rootsn = solver(a, b, c, &x1, &x2);

    ans(rootsn, x1, x2);

    return 0;
}

int solver(double a, double b, double c, double* x1, double* x2)    //решает уравнение
{
    if (fabs(a) < EPSILON)
    {
        return NotSquareSolver(b, c, x1);
    }
    else
    {
        return SquareSolver(a, b, c, x1, x2);
    }
}

int NotSquareSolver(double b, double c, double* x)  //случай если коэфицент a равен 0
{
    if (fabs(b) < EPSILON)
    {
        return (fabs(c) < EPSILON) ? SS_ENF : 0;
    }
    else
    {
        *x = MinusNullCheck(-c / b);
        return 1;
    }
}

int SquareSolver(double a, double b, double c, double* x1, double* x2)  //случай если коэфицент a не равен 0
{
    double d = b * b - 4 * a * c;

    if (d > 0)
    {
        *x1 = min(MinusNullCheck((-b - sqrt(d)) / 2 / a), MinusNullCheck((-b + sqrt(d)) / 2 / a));
        *x2 = max(MinusNullCheck((-b - sqrt(d)) / 2 / a), MinusNullCheck((-b + sqrt(d)) / 2 / a));
        return 2;
    }
    else if (fabs(d) < EPSILON)
    {
        *x1 = MinusNullCheck(-b / 2 / a);

        return 1;
    }
    else
    {
        return 0;
    }
}

void entering(double* a, double* b, double* c)  //ввод
{
    printf("Я решаю квадратные уравнения.\n");
    printf("Уравнение имеет вид ax^2 + bx + c = 0\n");
    printf("Введите коэфиценты a, b и c через пробел\n");

    int ent = 0;
    ent = scanf("%lf %lf %lf", a, b, c);
    while (ent != 3)
    {
        printf("Введите числа правильно\n");
        ent = scanf("%*s %lf %lf %lf", a, b, c);
    }
}

void ans(int rootsn, double x1, double x2)    //вывод
{
    switch (rootsn)
    {
        case 0 :
            printf("Уравнение не имеет корней.");
            break;
        case 1 :
            if (fabs(x1) < EPSILON)
            {
                x1 = 0.0;
            }
            printf("Уравнение имеет один корень: %g", x1);
            break;
        case 2 :
            printf("Уравнение имеет два корня: %g и %g", x1, x2);
            break;
        case SS_ENF :
            printf("Уравнение имеет бесконечное количество корней");
            break;
        default :
            printf("error");
    }
}

double MinusNullCheck(double num)   //меняет -0 на 0
{
    if (fabs(num) < EPSILON)
    {
        return fabs(num);
    }
    else
    {
        return num;
    }
}

int tester(void)    //тестирует solver()
{
    int nErrors = 0;

    nErrors += nTester(1, 0.0, 0.0, 0.0, NAN, NAN, SS_ENF);
    nErrors += nTester(2, 0.0, 0.0, 1.0, NAN, NAN, 0);
    nErrors += nTester(3, 0.0, 2.0, 0.0, 0.0, NAN, 1);
    nErrors += nTester(4, 0.0, 2.0, 3.0, -1.5, NAN, 1);
    nErrors += nTester(5, 0.0, 1.0, -5.0, 5.0, NAN, 1);
    nErrors += nTester(6, 0.0, 1.5, 2.25, -1.5, NAN, 1);
    nErrors += nTester(7, 2.0, 0.0, 0.0, 0.0, NAN, 1);
    nErrors += nTester(8, -3.0, 0.0, 27.0, -3.0, 3.0, 2);
    nErrors += nTester(9, 1.0, 0.0, 4.0, NAN, NAN, 0);
    nErrors += nTester(10, 0.7, 0.525, -1.75, -2.0, 1.25, 2);
    nErrors += nTester(11, 3.0, -6.0, 3.0, 1.0, NAN, 1);

    return nErrors;
}

int nTester(int nTest, double a, double b, double c, double x1Right, double x2Right, int rootsnRight)   //n-ый тест
{
    double x1 = NAN, x2 = NAN;
    int rootsn = solver(a, b, c, &x1, &x2);

    if (rootsn != rootsnRight || fabs(x1 - x1Right) > EPSILON || fabs(x2 - x2Right) > EPSILON)
    {
        printf("Error Test %d: a = %lg, b = %lg, c = %lg, x1 = %lg, x2 = %lg, rootsn = %d\n"
               "Expected: x1 = %lg, x2 = %lg, rootsn = %d\n",
               nTest, a, b, c, x1, x2, rootsn, x1Right, x2Right, rootsnRight);

        return 1;
    }
    else
    {
        return 0;
    }
}
