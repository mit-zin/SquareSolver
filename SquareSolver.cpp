#include <TXLib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

struct TestData
{
    int nTest;
    double a, b, c;
    double x1Right, x2Right;
    int rootsnRight;
};

int solver(double a, double b, double c, double* x1, double* x2);
int SquareSolver(double a, double b, double c, double* x1, double* x2);
int LinearSolver(double b, double c, double* x);
int input(double* a, double* b, double* c);
void ans(int rootsn, double x1, double x2);
double MinusNullCheck(double num);
int tester(void);
int nTester(TestData data);
bool DoubleZeroCheck(double num);
void SortRoots(double* x1, double* x2);
void BufferClear(void);

//Читать прату, указатели массивы строки и структуры

const double EPSILON = 0.000001;
const int SUCCESS = 0;
const int UNSUCCESS = 1;
const int NO_ROOTS = 0;
const int ONE_ROOT = 1;
const int TWO_ROOTS = 2;
const int INF_ROOTS = -1;
const int ONE_ERROR = 1;
const int NO_ERRORS = 0;

int main(void)
{
    int nErrors = tester();
    printf("Число ошибок: %d\n", nErrors);

    double a = NAN, b = NAN, c = NAN;

    if (input(&a, &b, &c) == SUCCESS)
    {
        double x1 = NAN, x2 = NAN;
        int rootsn = 0;

        rootsn = solver(a, b, c, &x1, &x2);

        ans(rootsn, x1, x2);
    }

    printf("Конец программы");

    return SUCCESS;
}

int solver(double a, double b, double c, double* x1, double* x2)    //решает уравнение
{
    assert(x1);
    assert(x2);

    if (DoubleZeroCheck(a))
    {
        return LinearSolver(b, c, x1);
    }
    else
    {
        return SquareSolver(a, b, c, x1, x2);
    }
}

int LinearSolver(double b, double c, double* x)  //случай если коэфицент a равен 0
{
    assert(x);

    if (DoubleZeroCheck(b))
    {
        return (DoubleZeroCheck(c)) ? INF_ROOTS : NO_ROOTS;
    }
    else
    {
        *x = MinusNullCheck(-c / b);
        return ONE_ROOT;
    }
}

int SquareSolver(double a, double b, double c, double* x1, double* x2)  //случай если коэфицент a не равен 0
{
    assert(!DoubleZeroCheck(a));
    assert(x1);
    assert(x2);

    double d = b * b - 4 * a * c;

    if (d > 0)
    {
        *x1 = min(MinusNullCheck((-b - sqrt(d)) / 2 / a), MinusNullCheck((-b + sqrt(d)) / 2 / a));
        *x2 = max(MinusNullCheck((-b - sqrt(d)) / 2 / a), MinusNullCheck((-b + sqrt(d)) / 2 / a));
        return TWO_ROOTS;
    }
    else if (DoubleZeroCheck(d))
    {
        *x1 = MinusNullCheck(-b / 2 / a);

        return ONE_ROOT;
    }
    else
    {
        return NO_ROOTS;
    }
}

int input(double* a, double* b, double* c)  //ввод
{
    assert(a);
    assert(b);
    assert(c);

    printf("Я решаю квадратные уравнения.\n");
    printf("Уравнение имеет вид ax^2 + bx + c = 0\n");
    printf("Введите коэфиценты a, b и c через пробел\n");

    int EofCheck = 0;

    while ((EofCheck = scanf("%lf %lf %lf", a, b, c)) != 3)
    {
        if (EofCheck == EOF)
        {
            printf("End of file\n");
            return UNSUCCESS;
        }
        BufferClear();
        printf("Введите числа правильно\n");
    }

    return SUCCESS;
}

void ans(int rootsn, double x1, double x2)    //вывод
{
    switch (rootsn)
    {
        case NO_ROOTS :
            printf("Уравнение не имеет корней.\n");
            break;
        case ONE_ROOT :
            printf("Уравнение имеет один корень: %g\n", x1);
            break;
        case TWO_ROOTS :
            printf("Уравнение имеет два корня: %g и %g\n", x1, x2);
            break;
        case INF_ROOTS :
            printf("Уравнение имеет бесконечное количество корней\n");
            break;
        default :
            printf("error\n");
    }
}

double MinusNullCheck(double num)   //меняет -0 на 0
{
    if (DoubleZeroCheck(num))
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

    struct TestData test1 = {1, 0.0, 0.0, 0.0, NAN, NAN, INF_ROOTS};
    struct TestData test2 = {2, 0.0, 0.0, 1.0, NAN, NAN, NO_ROOTS};
    struct TestData test3 = {3, 0.0, 2.0, 0.0, 0.0, NAN, ONE_ROOT};
    struct TestData test4 = {4, 0.0, 2.0, 3.0, -1.5, NAN, ONE_ROOT};
    struct TestData test5 = {5, 0.0, 1.0, -5.0, 5.0, NAN, ONE_ROOT};
    struct TestData test6 = {6, 0.0, 1.5, 2.25, -1.5, NAN, ONE_ROOT};
    struct TestData test7 = {7, 2.0, 0.0, 0.0, 0.0, NAN, ONE_ROOT};
    struct TestData test8 = {8, -3.0, 0.0, 27.0, 3.0, -3.0, TWO_ROOTS};
    struct TestData test9 = {9, 1.0, 0.0, 4.0, NAN, NAN, NO_ROOTS};
    struct TestData test10 = {10, 0.7, 0.525, -1.75, -2.0, 1.25, TWO_ROOTS};
    struct TestData test11 = {11, 3.0, -6.0, 3.0, 1.0, NAN, ONE_ROOT};

    nErrors += nTester(test1);
    nErrors += nTester(test2);
    nErrors += nTester(test3);
    nErrors += nTester(test4);
    nErrors += nTester(test5);
    nErrors += nTester(test6);
    nErrors += nTester(test7);
    nErrors += nTester(test8);
    nErrors += nTester(test9);
    nErrors += nTester(test10);
    nErrors += nTester(test11);

    return nErrors;
}

int nTester(TestData data)   //n-ый тест
{
    double x1 = NAN, x2 = NAN;
    int rootsn = solver(data.a, data.b, data.c, &x1, &x2);

    SortRoots(&(data.x1Right), &(data.x2Right));

    if (rootsn != data.rootsnRight || fabs(x1 - data.x1Right) > EPSILON || fabs(x2 - data.x2Right) > EPSILON)
    {
        printf("Error Test %d: a = %lg, b = %lg, c = %lg, x1 = %lg, x2 = %lg, rootsn = %d\n"
               "Expected: x1 = %lg, x2 = %lg, rootsn = %d\n",
               data.nTest, data.a, data.b, data.c, x1, x2, rootsn, data.x1Right, data.x2Right, data.rootsnRight);

        return ONE_ERROR;
    }
    else
    {
        return NO_ERRORS;
    }
}

bool DoubleZeroCheck(double num)    //проверяет, что переменная типа double равна нулю
{
    return fabs(num) < EPSILON;
}

void SortRoots(double* x1, double* x2)  //меняет корни местами, если x1 > x2
{
    if (*x1 > *x2)
    {
        double change = *x1;
        *x1 = *x2;
        *x2 = change;
    }
}

void BufferClear(void)
{
    while (getchar() != '\n');
}
