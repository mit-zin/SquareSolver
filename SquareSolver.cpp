#include <TXLib.h>
#include <math.h>
#include <stdio.h>

// âíåøíèé âèä òåêñòà â êîíñîëè
#define PRINTF_GREEN(...)        printf("\033[0;32m" __VA_ARGS__)
#define PRINTF_RED(...)          printf("\033[0;31m" __VA_ARGS__)
#define PRINTF_USUAL(...)        printf("\033[0m" __VA_ARGS__)
#define PRINTF_VIOL_B_YEL_F(...) printf("\033[0;33;45m" __VA_ARGS__) // ôèîëåòîâûé ôîí, æ¸ëòûé òåêñò
#define PRINTF_ITALICS(...)      printf("\033[0;3m" __VA_ARGS__)
#define PRINTF_RED_ITAL(...)     printf("\033[0;3;31m" __VA_ARGS__)

const double EPSILON          = 1e-6;

const int    LAST_SYMB_OF_ANS = 3;

enum testResult    {NO_ERRORS = 0, ONE_ERROR = 1};
enum roots         {INF_ROOTS = -1, NO_ROOTS = 0, ONE_ROOT = 1, TWO_ROOTS = 2};
enum result        {SUCCESS = 0, UNSUCCESS = 1};
enum yes_no        {NO = 0, YES = 1};
enum compareResult {SECOND_BIGGER = -1, EQUAL = 0, FIRST_BIGGER = 1};

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

enum roots         Solve         (struct SolverParameters *par);
enum roots         LinearSolve   (struct SolverParameters *par);
enum roots         SquareSolve   (struct SolverParameters *par);
int                SolverInput   (struct SolverParameters *par);
int                UnitTester    (void);
enum testResult    Test          (struct TestData data);
enum compareResult DoubleCompare (double num1, double num2);
double             MinusZeroCheck(double num);
void               SolverOutput  (enum roots nRoots, double x1, double x2);
void               SortRoots     (double* x1, double* x2);
void               BufferClear   (void);
void               ArrClear      (char arr[], int arSize);
enum result        Request       (enum yes_no * answer);
enum result        TestModule    (void);
enum result        SolverModule  (void);
enum result        SingleEquation(void);

int main(void)
{
    enum result status = TestModule();

    if (status == SUCCESS)
    {
        SolverModule();
    }

    PRINTF_USUAL("Êîíåö ïðîãðàììû");

    return SUCCESS;
}

// ÷àñòü ñâÿçàííàÿ ñ unit-òåñòàìè
enum result TestModule(void)
{
    PRINTF_USUAL("Âû æåëàåòå âêëþ÷èòü òåñòèðîâàíèå?");
    enum yes_no testReq = NO;

    if (Request(&testReq) == SUCCESS)
    {
        if (testReq == YES)
        {
            int nErrors = UnitTester();
            PRINTF_ITALICS("×èñëî îøèáîê: %d\n", nErrors);
        }

        return SUCCESS;
    }
    else
    {
        return UNSUCCESS;
    }
}

// ÷àñòü ñâÿçàííàÿ ñ ðåøåíèåì óðàâíåíèÿ
enum result SolverModule(void)
{
    PRINTF_USUAL("ß ðåøàþ êâàäðàòíûå óðàâíåíèÿ.\n");
    printf("Óðàâíåíèå èìååò âèä ax^2 + bx + c = 0\n");

    enum yes_no solvReq = YES;
    while (solvReq == YES)
    {
        if (SingleEquation() == UNSUCCESS)
        {
            return UNSUCCESS;
        }

        PRINTF_USUAL("Õîòèòå, ÷òîáû ÿ ðåøèë åù¸ îäíî óðàâíåíèå?");

        if (Request(&solvReq) == UNSUCCESS)
        {
            return UNSUCCESS;
        }
    }
    return SUCCESS;
}

// ââîä êîýôèöåíòîâ, ðåøåíèå è îòâåò äëÿ îäíîãî óðàâíåíèÿ
enum result SingleEquation(void)
{
    struct SolverParameters par = {NAN, NAN, NAN, NAN, NAN};

    if (SolverInput(&par) == SUCCESS)
    {
        enum roots nRoots = NO_ROOTS;

        nRoots = Solve(&par);

        SolverOutput(nRoots, par.x1, par.x2);

        return SUCCESS;
    }
    else
    {
        return UNSUCCESS;
    }
}

// ðåøàåò óðàâíåíèå
enum roots Solve(SolverParameters *par)
{
    assert(par);

    if (!DoubleCompare((*par).a, 0))
    {
        return LinearSolve(par);
    }
    else
    {
        return SquareSolve(par);
    }
}

// ñëó÷àé åñëè êîýôèöåíò a ðàâåí 0
enum roots LinearSolve(struct SolverParameters *par)
{
    assert(par);

    if (!DoubleCompare((*par).b, 0))
    {
        return (!DoubleCompare((*par).c, 0)) ? INF_ROOTS : NO_ROOTS;
    }
    else
    {
        (*par).x1 = MinusZeroCheck(-(*par).c / (*par).b);
        return ONE_ROOT;
    }
}

// ñëó÷àé åñëè êîýôèöåíò a íå ðàâåí 0
enum roots SquareSolve(SolverParameters *par)
{
    assert(par);
    assert(DoubleCompare((*par).a, 0));

    double d = (*par).b * (*par).b - 4 * (*par).a * (*par).c;

    if (d > 0)
    {
        (*par).x1 = min(MinusZeroCheck((-(*par).b - sqrt(d)) / 2 / (*par).a),
                        MinusZeroCheck((-(*par).b + sqrt(d)) / 2 / (*par).a));
        (*par).x2 = max(MinusZeroCheck((-(*par).b - sqrt(d)) / 2 / (*par).a),
                        MinusZeroCheck((-(*par).b + sqrt(d)) / 2 / (*par).a));

        return TWO_ROOTS;
    }
    else if (!DoubleCompare(d, 0))
    {
        (*par).x1 = MinusZeroCheck(-(*par).b / 2 / (*par).a);

        return ONE_ROOT;
    }
    else
    {
        return NO_ROOTS;
    }
}

// ââîä
int SolverInput(struct SolverParameters *par)
{
    assert(par);

    PRINTF_USUAL("Ââåäèòå êîýôèöåíòû a, b è c ÷åðåç ïðîáåë\n");
    PRINTF_VIOL_B_YEL_F();

    int EofCheck = 0;

    while ((EofCheck = scanf("%lf %lf %lf", &(*par).a, &(*par).b, &(*par).c)) != 3)
    {
        if (EofCheck == EOF)
        {
            PRINTF_ITALICS("Êîíåö ôàéëà\n");
            return UNSUCCESS;
        }
        BufferClear();
        PRINTF_RED("Ââåäèòå ÷èñëà ïðàâèëüíî\n");
        PRINTF_VIOL_B_YEL_F();
    }
    BufferClear();

    return SUCCESS;
}

// âûâîä îòâåòà
void SolverOutput(enum roots nRoots, double x1, double x2)
{
    PRINTF_GREEN();

    switch (nRoots)
    {
        case NO_ROOTS :
            printf("Óðàâíåíèå íå èìååò êîðíåé.\n");
            break;
        case ONE_ROOT :
            printf("Óðàâíåíèå èìååò îäèí êîðåíü: %g\n", x1);
            break;
        case TWO_ROOTS :
            printf("Óðàâíåíèå èìååò äâà êîðíÿ: %g è %g\n", x1, x2);
            break;
        case INF_ROOTS :
            printf("Óðàâíåíèå èìååò áåñêîíå÷íîå êîëè÷åñòâî êîðíåé\n" );
            break;
        default :
            PRINTF_RED_ITAL("error\n");
    }
}

// ìåíÿåò -0 íà 0
double MinusZeroCheck(double num)
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

// çàïðîñ íà îòâåò äà èëè íåò
enum result Request(enum yes_no * answer)
{
    assert(answer);

    PRINTF_USUAL(" Ââåäèòå \"äà\" èëè \"íåò\".\n");
    PRINTF_VIOL_B_YEL_F();

    const int SIZE_ANS = 5;
    char input[SIZE_ANS] = {};

    char *eofCheck = input;
    eofCheck = fgets(input, SIZE_ANS, stdin);

    while (strcmp(input, "äà\n") && strcmp(input, "íåò\n"))
    {
        if (!eofCheck)
        {
            PRINTF_ITALICS("Êîíåö ôàéëà\n");

            return UNSUCCESS;
        }

        PRINTF_RED_ITAL("Ìîæíî ââîäèòü òîëüêî \"äà\" èëè \"íåò\".\n");
        PRINTF_VIOL_B_YEL_F();

        if (input[LAST_SYMB_OF_ANS] != '\0' && input[LAST_SYMB_OF_ANS] != '\n')
        {
            BufferClear();
        }

        ArrClear(input, SIZE_ANS);

        eofCheck = fgets(input, SIZE_ANS, stdin);
    }

    if (!strcmp(input, "äà\n"))
        *answer = YES;
    else
        *answer = NO;

    return SUCCESS;
}

// òåñòèðóåò Solve
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
    {
        nErrors += Test(tests[i]);
    }

    return nErrors;
}

// n-ûé òåñò
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
    {
        return NO_ERRORS;
    }
}

// ñðàâíèâàåò äâà ÷èñëà òèïà double
enum compareResult DoubleCompare(double num1, double num2)
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

// ìåíÿåò êîðíè ìåñòàìè, åñëè x1 > x2
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

// î÷èñòêà áóôåðà
void BufferClear(void)
{
    while (getchar() != '\n');
}

// ÷èñòèò ìàññèâ
void ArrClear(char arr[], int arSize)
{
    for (int i = 0; i < arSize; i++)
    {
        arr[i] = '\0';
    }
}
