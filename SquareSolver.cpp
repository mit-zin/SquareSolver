#include <TXLib.h>
#include <math.h>
#include <stdio.h>

// внешний вид текста в консоли
#define PRINTF_USUAL(...)              printf("\033[0m" __VA_ARGS__)
#define PRINTF_GREEN(...)              printf("\033[0;32m" __VA_ARGS__)
#define PRINTF_RED(...)                printf("\033[0;31m" __VA_ARGS__)
#define PRINTF_VIOL_BACK_YEL_FORE(...) printf("\033[0;33;45m" __VA_ARGS__)
#define PRINTF_ITALICS(...)            printf("\033[0;3m" __VA_ARGS__)
#define PRINTF_RED_ITAL(...)           printf("\033[0;3;31m" __VA_ARGS__)

const double EPSILON = 1e-6;

enum testResult    {NO_ERRORS = 0,
                    ONE_ERROR = 1};

enum roots         {INF_ROOTS = -1,
                    NO_ROOTS  =  0,
                    ONE_ROOT  =  1,
                    TWO_ROOTS =  2};

enum result        {SUCCESS   = 0,
                    UNSUCCESS = 1};

enum yes_no        {NO  = 0,
                    YES = 1};

enum compareResult {SECOND_BIGGER = -1,
                    EQUAL         =  0,
                    FIRST_BIGGER  =  1};

struct TestData
{
    double a, b, c;
    double x1Right, x2Right;
    enum roots nRootsRight;
    int nTest;
};

struct SolverParameters
{
    double a, b, c;
    double x1, x2;
};

enum result        SolverModule  (void);
enum result        SingleEquation(void);
int                SolverInput   (struct SolverParameters *par);
void               SolverOutput  (enum roots nRoots, double x1, double x2);//
enum roots         Solve         (struct SolverParameters *par);
enum roots         LinearSolve   (struct SolverParameters *par);
enum roots         SquareSolve   (struct SolverParameters *par);

enum result        TestModule    (void);
int                UnitTester    (void);
enum testResult    Test          (struct TestData data);
void               SortRoots     (double* x1, double* x2);

void               ArrClear      (char arr[], int arSize);
enum result        Request       (enum yes_no * answer);

enum compareResult DoubleCompare (double num1, double num2);

double             MinusZeroCheck(double num);

void               BufferClear   (void);

int main(void)
{
    enum result status = TestModule();

    if (status == SUCCESS)
        SolverModule();

    PRINTF_USUAL("Конец программы");

    return SUCCESS;
}

// часть связанная с unit-тестами
enum result TestModule(void)
{
    PRINTF_USUAL("Вы желаете включить тестирование?");
    enum yes_no testReq = NO;

    if (Request(&testReq) == SUCCESS)
    {
        if (testReq == YES)
        {
            int nErrors = UnitTester();
            PRINTF_ITALICS("Число ошибок: %d\n", nErrors);
        }

        return SUCCESS;
    }
    else
        return UNSUCCESS;
}

// часть связанная с решением уравнения
enum result SolverModule(void)
{
    PRINTF_USUAL("Я решаю квадратные уравнения.\n");
    printf("Уравнение имеет вид ax^2 + bx + c = 0\n");

    enum yes_no solvReq = YES;
    while (solvReq == YES)
    {
        if (SingleEquation() == UNSUCCESS)
            return UNSUCCESS;

        PRINTF_USUAL("Хотите, чтобы я решил ещё одно уравнение?");

        if (Request(&solvReq) == UNSUCCESS)
            return UNSUCCESS;
    }

    return SUCCESS;
}

// ввод коэфицентов, решение и ответ для одного уравнения
enum result SingleEquation(void)
{
    struct SolverParameters par = {NAN, NAN, NAN, NAN, NAN};

    if (SolverInput(&par) == SUCCESS)
    {
        enum roots nRoots = Solve(&par);

        SolverOutput(nRoots, par.x1, par.x2);

        return SUCCESS;
    }
    else
        return UNSUCCESS;
}

// решает уравнение
enum roots Solve(SolverParameters *par)
{
    assert(par);

    return (DoubleCompare((*par).a, 0) != EQUAL) ? SquareSolve(par) : LinearSolve(par);
}

// случай если коэфицент a равен 0
enum roots LinearSolve(struct SolverParameters *par)
{
    assert(par);

    if (DoubleCompare((*par).b, 0) == EQUAL)
        return (DoubleCompare((*par).c, 0) == EQUAL) ? INF_ROOTS : NO_ROOTS;
    else
    {
        (*par).x1 = MinusZeroCheck(-(*par).c / (*par).b);

        return ONE_ROOT;
    }
}

// случай если коэфицент a не равен 0
enum roots SquareSolve(SolverParameters *par)
{
    assert(par);
    assert(DoubleCompare((*par).a, 0));

    double d = (*par).b * (*par).b - 4 * (*par).a * (*par).c;
    enum compareResult res = DoubleCompare(d, 0);

    if (res == FIRST_BIGGER)
    {
        (*par).x1 = min(MinusZeroCheck((-(*par).b - sqrt(d)) / (2 * (*par).a)),
                        MinusZeroCheck((-(*par).b + sqrt(d)) / (2 * (*par).a)));
        (*par).x2 = max(MinusZeroCheck((-(*par).b - sqrt(d)) / (2 * (*par).a)),
                        MinusZeroCheck((-(*par).b + sqrt(d)) / (2 * (*par).a)));

        return TWO_ROOTS;
    }
    else if (res == EQUAL)
    {
        (*par).x1 = MinusZeroCheck(-(*par).b / 2 / (*par).a);

        return ONE_ROOT;
    }
    else
        return NO_ROOTS;
}

// ввод
int SolverInput(struct SolverParameters *par)
{
    assert(par);

    PRINTF_USUAL("Введите коэфиценты a, b и c через пробел\n");
    PRINTF_VIOL_BACK_YEL_FORE();

    int EofCheck = 0;

    while ((EofCheck = scanf("%lf %lf %lf", &(*par).a, &(*par).b, &(*par).c)) != 3)
    {
        if (EofCheck == EOF)
        {
            PRINTF_ITALICS("Конец файла\n");

            return UNSUCCESS;
        }
        BufferClear();

        PRINTF_RED("Введите числа правильно\n");
        PRINTF_VIOL_BACK_YEL_FORE();
    }
    BufferClear();

    return SUCCESS;
}

// вывод ответа
void SolverOutput(enum roots nRoots, double x1, double x2)
{
    PRINTF_GREEN();

    switch (nRoots)
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
            PRINTF_RED_ITAL("error\n");
    }
}

// меняет -0 на 0
double MinusZeroCheck(double num)
{
    return (DoubleCompare(num, 0) == EQUAL) ? fabs(num) : num;
}

// запрос на ответ да или нет
enum result Request(enum yes_no * answer)
{
    assert(answer);

    PRINTF_USUAL(" Введите \"да\" или \"нет\" (без пробелов или других символов).\n");
    PRINTF_VIOL_BACK_YEL_FORE();

    const int LAST_SYMB_OF_ANS = 3;
    const int SIZE_ANS         = 5;

    char input[SIZE_ANS] = {};

    char *eofCheck = fgets(input, SIZE_ANS, stdin);

    while (strcmp(input, "да\n") && strcmp(input, "нет\n"))
    {
        if (!eofCheck)
        {
            PRINTF_ITALICS("Конец файла\n");

            return UNSUCCESS;
        }

        PRINTF_RED("Можно вводить только \"да\" или \"нет\".\n");
        PRINTF_VIOL_BACK_YEL_FORE();

        if (input[LAST_SYMB_OF_ANS] != '\0' && input[LAST_SYMB_OF_ANS] != '\n')
            BufferClear();

        ArrClear(input, SIZE_ANS);

        eofCheck = fgets(input, SIZE_ANS, stdin);
    }

    *answer = (!strcmp(input, "да\n")) ? YES : NO;

    return SUCCESS;
}

// тестирует Solve
int UnitTester(void)
{
    int nErrors = 0;

    struct TestData tests[] = {{0.0, 0.0, 0.0, NAN, NAN, INF_ROOTS, 1},
                               {0.0, 0.0, 1.0, NAN, NAN, NO_ROOTS, 2},
                               {0.0, 2.0, 0.0, 0.0, NAN, ONE_ROOT, 3},
                               {0.0, 2.0, 3.0, -1.5, NAN, ONE_ROOT, 4},
                               {0.0, 1.0, -5.0, 5.0, NAN, ONE_ROOT, 5},
                               {0.0, 1.5, 2.25, -1.5, NAN, ONE_ROOT, 6},
                               {2.0, 0.0, 0.0, 0.0, NAN, ONE_ROOT, 7},
                               {-3.0, 0.0, 27.0, 3.0, -3.0, TWO_ROOTS, 8},
                               {1.0, 0.0, 4.0, NAN, NAN, NO_ROOTS, 9},
                               {0.7, 0.525, -1.75, -2.0, 1.25, TWO_ROOTS, 10},
                               {3.0, -6.0, 3.0, 1.0, NAN, ONE_ROOT, 11}};

    int nTests = sizeof tests / sizeof (struct TestData);

    for (int i = 0; i < nTests; i++)
        nErrors += Test(tests[i]);

    return nErrors;
}

// n-ый тест
enum testResult Test(struct TestData data)
{
    struct SolverParameters par = {data.a, data.b, data.c, NAN, NAN};
    enum roots nRoots = Solve(&par);

    SortRoots(&(data.x1Right), &(data.x2Right));

    if (nRoots != data.nRootsRight ||
        DoubleCompare(par.x1, data.x1Right) ||
        DoubleCompare(par.x2, data.x2Right))
    {
        PRINTF_RED_ITAL("Error Test %d: a = %lg, b = %lg, c = %lg, x1 = %lg, x2 = %lg, nRoots = %d\n"
                        "Expected: x1 = %lg, x2 = %lg, nRoots = %d\n",
                    data.nTest, data.a,
                    data.b, data.c,
                    par.x1, par.x2,
                    nRoots, data.x1Right,
                    data.x2Right, data.nRootsRight);

        return ONE_ERROR;
    }
    else
        return NO_ERRORS;
}

// сравнивает два числа типа double
enum compareResult DoubleCompare(double num1, double num2)
{
    if (num1 - num2 > EPSILON)
        return FIRST_BIGGER;
    else if (num1 - num2 < -EPSILON)
        return SECOND_BIGGER;
    else
        return EQUAL;
}

// меняет корни местами, если x1 > x2
void SortRoots(double* x1, double* x2)
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

// очистка буфера
void BufferClear(void)
{
    while (getchar() != '\n');
}

// чистит массив
void ArrClear(char arr[], int arSize)
{
    for (int i = 0; i < arSize; i++)
        arr[i] = '\0';
}
