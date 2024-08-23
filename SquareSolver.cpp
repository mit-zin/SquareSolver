#include <TXLib.h>
#include <stdio.h>
#include <math.h>

//Читать прату
//Глава про структуры данных
//Глава про препроцессинг
//как делать цветной вывод в printf \033[] #define
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define USUAL "\033[0m"
#define ITALICS "\033[0;3m"
#define VIOL_B_YEL_F "\033[0;45;33m"
#define RED_ITAL "\033[0;3;31m"

const double EPSILON       = 0.000001;
const int    SUCCESS       = 0;
const int    UNSUCCESS     = 1;
/*const int    NO_ROOTS      = 0;
const int    ONE_ROOT      = 1;
const int    TWO_ROOTS     = 2;
const int    INF_ROOTS     = -1;*/
const int    ONE_ERROR     = 1;
const int    NO_ERRORS     = 0;
const int    FIRST_BIGGER  = 1;
const int    SECOND_BIGGER = -1;
const int    EQUAL         = 0;
const int    NUM_OF_TESTS  = 11;

enum roots {infRoots = -1, noRoots, oneRoot, twoRoots};

struct TestData
{
    int nTest;
    double a, b, c;
    double x1Right, x2Right;
    enum roots nRootsRight;
};

struct SolverParameters
{
    double a, b, c;
    double x1, x2;
};

enum roots solver        (struct SolverParameters *par);
enum roots LinearSolver  (struct SolverParameters *par);
enum roots SquareSolver  (struct SolverParameters *par);
int        input         (struct SolverParameters *par);
int        unitTester    (void);
int        nTester       (struct TestData data);
int        DoubleCompare (double num1, double num2);
double     MinusNullCheck(double num);
void       ans           (enum roots nRoots, double x1, double x2);
void       SortRoots     (double* x1, double* x2);
void       BufferClear   (void);

//Читать прату, указатели массивы строки и структуры

int main(void)
{
    int nErrors = unitTester();
    printf(ITALICS "Число ошибок: %d\n" USUAL, nErrors);

    struct SolverParameters par = {NAN, NAN, NAN, NAN, NAN};

    if (input(&par) == SUCCESS)
    {
        //int rootsn = 0;
        enum roots nRoots = noRoots;

        nRoots = solver(&par);

        ans(nRoots, par.x1, par.x2);
    }

    printf(USUAL "Конец программы");

    return SUCCESS;
}

enum roots solver(SolverParameters *par)    //решает уравнение
{
    assert(par);

    if (!DoubleCompare((*par).a, 0))
    {
        return LinearSolver(par);
    }
    else
    {
        return SquareSolver(par);
    }
}

enum roots LinearSolver(struct SolverParameters *par)  //случай если коэфицент a равен 0
{
    assert(par);

    if (!DoubleCompare((*par).b, 0))
    {
        return (!DoubleCompare((*par).c, 0)) ? infRoots : noRoots;
    }
    else
    {
        (*par).x1 = MinusNullCheck(-(*par).c / (*par).b);
        return oneRoot;
    }
}

enum roots SquareSolver(SolverParameters *par)  //случай если коэфицент a не равен 0
{
    assert(DoubleCompare((*par).a, 0));
    assert(par);

    double d = (*par).b * (*par).b - 4 * (*par).a * (*par).c;

    if (d > 0)
    {
        (*par).x1 = min(MinusNullCheck((-(*par).b - sqrt(d)) / 2 / (*par).a),
                        MinusNullCheck((-(*par).b + sqrt(d)) / 2 / (*par).a));
        (*par).x2 = max(MinusNullCheck((-(*par).b - sqrt(d)) / 2 / (*par).a),
                        MinusNullCheck((-(*par).b + sqrt(d)) / 2 / (*par).a));

        return twoRoots;
    }
    else if (!DoubleCompare(d, 0))
    {
        (*par).x1 = MinusNullCheck(-(*par).b / 2 / (*par).a);

        return oneRoot;
    }
    else
    {
        return noRoots;
    }
}

int input(struct SolverParameters *par)  //ввод
{
    assert(par);

    printf(USUAL "Я решаю квадратные уравнения.\n");
    printf("Уравнение имеет вид ax^2 + bx + c = 0\n");
    printf("Введите коэфиценты a, b и c через пробел\n" VIOL_B_YEL_F);

    int EofCheck = 0;

    while ((EofCheck = scanf("%lf %lf %lf", &(*par).a, &(*par).b, &(*par).c)) != 3)
    {
        if (EofCheck == EOF)
        {
            printf(ITALICS "Конец файла\n");
            return UNSUCCESS;
        }
        BufferClear();
        printf(RED "Введите числа правильно\n" VIOL_B_YEL_F);
    }

    return SUCCESS;
}

void ans(enum roots nRoots, double x1, double x2)    //вывод ответа
{
    switch (nRoots)
    {
        case noRoots :
            printf(GREEN "Уравнение не имеет корней.\n");
            break;
        case oneRoot :
            printf(GREEN "Уравнение имеет один корень: %g\n", x1);
            break;
        case twoRoots :
            printf(GREEN "Уравнение имеет два корня: %g и %g\n", x1, x2);
            break;
        case infRoots :
            printf(GREEN "Уравнение имеет бесконечное количество корней\n");
            break;
        default :
            printf("error\n");
    }
}

double MinusNullCheck(double num)   //меняет -0 на 0
{
    if (!DoubleCompare(num, 0))
    {
        return fabs(num);
    }
    else
    {
        return num;
    }
}

int unitTester(void)    //тестирует solver()
{
    int nErrors = 0;

    struct TestData tests[NUM_OF_TESTS] = {{1, 0.0, 0.0, 0.0, NAN, NAN, infRoots},
                                           {2, 0.0, 0.0, 1.0, NAN, NAN, noRoots},
                                           {3, 0.0, 2.0, 0.0, 0.0, NAN, oneRoot},
                                           {4, 0.0, 2.0, 3.0, -1.5, NAN, oneRoot},
                                           {5, 0.0, 1.0, -5.0, 5.0, NAN, oneRoot},
                                           {6, 0.0, 1.5, 2.25, -1.5, NAN, oneRoot},
                                           {7, 2.0, 0.0, 0.0, 0.0, NAN, oneRoot},
                                           {8, -3.0, 0.0, 27.0, 3.0, -3.0, twoRoots},
                                           {9, 1.0, 0.0, 4.0, NAN, NAN, noRoots},
                                           {10, 0.7, 0.525, -1.75, -2.0, 1.25, twoRoots},
                                           {11, 3.0, -6.0, 3.0, 1.0, NAN, oneRoot}};

    for (int i = 0; i < NUM_OF_TESTS; i++)
    {
        nErrors += nTester(tests[i]);
    }

    return nErrors;
}

int nTester(struct TestData data)   //n-ый тест
{
    struct SolverParameters par = {data.a, data.b, data.c, NAN, NAN};
    enum roots nRoots = solver(&par);

    SortRoots(&(data.x1Right), &(data.x2Right));

    if (nRoots != data.nRootsRight || DoubleCompare(par.x1, data.x1Right) || DoubleCompare(par.x2, data.x2Right))
    {
        printf(RED_ITAL "Error Test %d: a = %lg, b = %lg, c = %lg, x1 = %lg, x2 = %lg, nRoots = %d\n"
               "Expected: x1 = %lg, x2 = %lg, nRoots = %d\n" USUAL,
               data.nTest, data.a, data.b, data.c, par.x1, par.x2, nRoots, data.x1Right, data.x2Right, data.nRootsRight);

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
