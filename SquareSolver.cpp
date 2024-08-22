#include <TXLib.h>
#include <stdio.h>
#include <math.h>

struct TestData
{
    int nTest;
    double a, b, c;
    double x1Right, x2Right;
    int rootsnRight;
};

struct SolverParameters
{
    double a, b, c;
    double x1, x2;
};

int    solver        (struct SolverParameters *par);
int    LinearSolver  (struct SolverParameters *par);
int    SquareSolver  (struct SolverParameters *par);
int    input         (struct SolverParameters *par);
int    tester        (void);
int    nTester       (struct TestData data);
int    DoubleCompare (double num1, double num2);
double MinusNullCheck(double num);
void   ans           (int rootsn, double x1, double x2);
void   SortRoots     (double* x1, double* x2);
void   BufferClear   (void);

//Читать прату, указатели массивы строки и структуры

const double EPSILON       = 0.000001;
const int    SUCCESS       = 0;
const int    UNSUCCESS     = 1;
const int    NO_ROOTS      = 0;
const int    ONE_ROOT      = 1;
const int    TWO_ROOTS     = 2;
const int    INF_ROOTS     = -1;
const int    ONE_ERROR     = 1;
const int    NO_ERRORS     = 0;
const int    FIRST_BIGGER  = 1;
const int    SECOND_BIGGER = -1;
const int    EQUAL         = 0;

int main(void)
{
    int nErrors = tester();
    printf("Число ошибок: %d\n", nErrors);

    struct SolverParameters par = {NAN, NAN, NAN, NAN, NAN};

    if (input(&par) == SUCCESS)
    {
        int rootsn = 0;

        rootsn = solver(&par);

        ans(rootsn, par.x1, par.x2);
    }

    printf("Конец программы");

    return SUCCESS;
}

int solver(SolverParameters *par)    //решает уравнение
{
    assert(par);

    if (!DoubleCompare((*par).a, 0.0))
    {
        return LinearSolver(par);
    }
    else
    {
        return SquareSolver(par);
    }
}

int LinearSolver(struct SolverParameters *par)  //случай если коэфицент a равен 0
{
    assert(par);

    if (!DoubleCompare((*par).b, 0.0))
    {
        return (!DoubleCompare((*par).c, 0.0)) ? INF_ROOTS : NO_ROOTS;
    }
    else
    {
        (*par).x1 = MinusNullCheck(-(*par).c / (*par).b);
        return ONE_ROOT;
    }
}

int SquareSolver(SolverParameters *par)  //случай если коэфицент a не равен 0
{
    assert(DoubleCompare((*par).a, 0.0));
    assert(par);

    double d = (*par).b * (*par).b - 4 * (*par).a * (*par).c;

    if (d > 0)
    {
        (*par).x1 = min(MinusNullCheck((-(*par).b - sqrt(d)) / 2 / (*par).a),
                        MinusNullCheck((-(*par).b + sqrt(d)) / 2 / (*par).a));
        (*par).x2 = max(MinusNullCheck((-(*par).b - sqrt(d)) / 2 / (*par).a),
                        MinusNullCheck((-(*par).b + sqrt(d)) / 2 / (*par).a));
        return TWO_ROOTS;
    }
    else if (!DoubleCompare(d, 0.0))
    {
        (*par).x1 = MinusNullCheck(-(*par).b / 2 / (*par).a);

        return ONE_ROOT;
    }
    else
    {
        return NO_ROOTS;
    }
}

int input(struct SolverParameters *par)  //ввод
{
    assert(par);

    printf("Я решаю квадратные уравнения.\n");
    printf("Уравнение имеет вид ax^2 + bx + c = 0\n");
    printf("Введите коэфиценты a, b и c через пробел\n");

    int EofCheck = 0;

    while ((EofCheck = scanf("%lf %lf %lf", &(*par).a, &(*par).b, &(*par).c)) != 3)
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

void ans(int rootsn, double x1, double x2)    //вывод ответа
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
    if (!DoubleCompare(num, 0.0))
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

int nTester(struct TestData data)   //n-ый тест
{
    struct SolverParameters par = {data.a, data.b, data.c, NAN, NAN};
    int rootsn = solver(&par);

    SortRoots(&(data.x1Right), &(data.x2Right));

    if (rootsn != data.rootsnRight || DoubleCompare(par.x1, data.x1Right) || DoubleCompare(par.x2, data.x2Right))
    {
        printf("Error Test %d: a = %lg, b = %lg, c = %lg, x1 = %lg, x2 = %lg, rootsn = %d\n"
               "Expected: x1 = %lg, x2 = %lg, rootsn = %d\n",
               data.nTest, data.a, data.b, data.c, par.x1, par.x2, rootsn, data.x1Right, data.x2Right, data.rootsnRight);

        return ONE_ERROR;
    }
    else
    {
        return NO_ERRORS;
    }
}

int DoubleCompare(double num1, double num2)    //сравнивает два числа типа double
{
    if (num1 - num2 > EPSILON)
    {
        return FIRST_BIGGER;
    }
    else if (num1 - num2 < -EPSILON)
    {
        return SECOND_BIGGER;
    }
    else
    {
        return EQUAL;
    }
}


void SortRoots(double* x1, double* x2)  //меняет корни местами, если x1 > x2
{
    assert(x1);
    assert(x2);

    if (*x1 > *x2)
    {
        double change = *x1;
        *x1 = *x2;
        *x2 = change;
    }
}

void BufferClear(void)  //очистка буфера
{
    while (getchar() != '\n');
}
