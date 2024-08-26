#include <TXLib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

// ������� ��� ������ � �������
#define PRINTF_USUAL(...)              printf("\033[0m" __VA_ARGS__)
#define PRINTF_GREEN(...)              printf("\033[0;32m" __VA_ARGS__)
#define PRINTF_RED(...)                printf("\033[0;31m" __VA_ARGS__)
#define PRINTF_VIOL_BACK_YEL_FORE(...) printf("\033[0;33;45m" __VA_ARGS__)
#define PRINTF_ITALICS(...)            printf("\033[0;3m" __VA_ARGS__)
#define PRINTF_RED_ITAL(...)           printf("\033[0;3;31m" __VA_ARGS__)

const double EPSILON = 1e-6;

const int ROOTS_SIZE = 4;
const int MAX_STROOTS_LEN = 10;
const char stRoots[4][MAX_STROOTS_LEN] = {"NO_ROOTS", "ONE_ROOT", "TWO_ROOTS", "INF_ROOTS"};

enum testResult    {NO_ERRORS = 0,
                    ONE_ERROR = 1};

enum roots         {NO_ROOTS  = 0,
                    ONE_ROOT  = 1,
                    TWO_ROOTS = 2,
                    INF_ROOTS = 3};

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

enum result        SolverModule   (void);
enum result        SingleEquation (void);
int                SolverInput    (struct SolverParameters *par);
void               SolverOutput   (enum roots nRoots, double x1, double x2);
enum roots         Solve          (struct SolverParameters *par);
enum roots         LinearSolve    (struct SolverParameters *par);
enum roots         SquareSolve    (struct SolverParameters *par);

enum result        TestModule     (void);
int                UnitTester     (int *nErrors);
enum result        FileRead       (struct TestData data[], size_t arSize, FILE * fp);
enum result        xRightRead     (FILE *fp, double *xRight);
enum result        nRootsRightRead(FILE *fp, enum roots *nRootsRight);
enum testResult    Test           (struct TestData data);
void               SortRoots      (double* x1, double* x2);

void               ArrClear       (char arr[], int arSize);
enum result        Request        (enum yes_no * answer);

enum compareResult DoubleCompare  (double num1, double num2);

double             MinusZeroCheck (double num);

void               BufferClear    (void);

int main(void)
{
    enum result status = TestModule();

    if (status == SUCCESS)
        SolverModule();

    PRINTF_USUAL("����� ���������");

    return SUCCESS;
}

// ����� ��������� � unit-�������
enum result TestModule(void)
{
    PRINTF_USUAL("�� ������� �������� ������������?");
    enum yes_no testReq = NO;

    if (Request(&testReq) == SUCCESS)
    {
        if (testReq == YES)
        {
            int nErrors = 0;

            if (UnitTester(&nErrors) == UNSUCCESS)
            {
                PRINTF_RED_ITAL("������ ��� ������ �����.\n");

                return UNSUCCESS;
            }
            PRINTF_ITALICS("����� ������: %d\n", nErrors);
        }

        return SUCCESS;
    }
    else
        return UNSUCCESS;
}

// ����� ��������� � �������� ���������
enum result SolverModule(void)
{
    PRINTF_USUAL("� ����� ���������� ���������.\n");
    printf("��������� ����� ��� ax^2 + bx + c = 0\n");

    enum yes_no solvReq = YES;
    while (solvReq == YES)
    {
        if (SingleEquation() == UNSUCCESS)
            return UNSUCCESS;

        PRINTF_USUAL("������, ����� � ����� ��� ���� ���������?");

        if (Request(&solvReq) == UNSUCCESS)
            return UNSUCCESS;
    }

    return SUCCESS;
}

// ���� �����������, ������� � ����� ��� ������ ���������
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

// ������ ���������
enum roots Solve(SolverParameters *par)
{
    assert(par);

    return (DoubleCompare((*par).a, 0) != EQUAL) ? SquareSolve(par) : LinearSolve(par);
}

// ������ ���� ��������� a ����� 0
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

// ������ ���� ��������� a �� ����� 0
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

// ����
int SolverInput(struct SolverParameters *par)
{
    assert(par);

    PRINTF_USUAL("������� ���������� a, b � c ����� ������\n");
    PRINTF_VIOL_BACK_YEL_FORE();

    int EofCheck = 0;

    while ((EofCheck = scanf("%lf %lf %lf", &(*par).a, &(*par).b, &(*par).c)) != 3)
    {
        if (EofCheck == EOF)
        {
            PRINTF_ITALICS("����� �����\n");

            return UNSUCCESS;
        }
        BufferClear();

        PRINTF_RED("������� ����� ���������\n");
        PRINTF_VIOL_BACK_YEL_FORE();
    }
    BufferClear();

    return SUCCESS;
}

// ����� ������
void SolverOutput(enum roots nRoots, double x1, double x2)
{
    PRINTF_GREEN();

    switch (nRoots)
    {
        case NO_ROOTS :
            printf("��������� �� ����� ������.\n");
            break;
        case ONE_ROOT :
            printf("��������� ����� ���� ������: %g\n", x1);
            break;
        case TWO_ROOTS :
            printf("��������� ����� ��� �����: %g � %g\n", x1, x2);
            break;
        case INF_ROOTS :
            printf("��������� ����� ����������� ���������� ������\n");
            break;
        default :
            PRINTF_RED_ITAL("error\n");
    }
}

// ������ -0 �� 0
double MinusZeroCheck(double num)
{
    return (DoubleCompare(num, 0) == EQUAL) ? fabs(num) : num;
}

// ������ �� ����� �� ��� ���
enum result Request(enum yes_no * answer)
{
    assert(answer);

    PRINTF_USUAL(" ������� \"��\" ��� \"���\" (��� �������� ��� ������ ��������).\n");
    PRINTF_VIOL_BACK_YEL_FORE();

    const int LAST_SYMB_OF_ANS = 3;
    const int SIZE_ANS         = 5;

    char input[SIZE_ANS] = {};

    char *eofCheck = fgets(input, SIZE_ANS, stdin);

    while (strcmp(input, "��\n") && strcmp(input, "���\n"))
    {
        if (!eofCheck)
        {
            PRINTF_ITALICS("����� �����\n");

            return UNSUCCESS;
        }

        PRINTF_RED("����� ������� ������ \"��\" ��� \"���\".\n");
        PRINTF_VIOL_BACK_YEL_FORE();

        if (input[LAST_SYMB_OF_ANS] != '\0' && input[LAST_SYMB_OF_ANS] != '\n')
            BufferClear();

        ArrClear(input, SIZE_ANS);

        eofCheck = fgets(input, SIZE_ANS, stdin);
    }

    *answer = (!strcmp(input, "��\n")) ? YES : NO;

    return SUCCESS;
}

// ��������� Solve
int UnitTester(int *nErrors)
{
    FILE *fp = fopen("data.txt", "r");

    size_t arSize = 0;

    fscanf(fp, "%zd", &arSize);

    struct TestData tests[arSize];

    if (FileRead(tests, arSize, fp) == SUCCESS)
    {
        fclose(fp);

        for (size_t i = 0; i < arSize; i++)
            *nErrors += Test(tests[i]);

        return SUCCESS;
    }
    else
    {
        fclose(fp);

        return UNSUCCESS;
    }
}

// reads data in a file
enum result FileRead(struct TestData data[], size_t arSize, FILE *fp)
{
    bool res = true;

    for (size_t i = 0; i < arSize; i++)
    {
        res = res & (fscanf(fp, "%d %lf %lf %lf",
                           &data[i].nTest, &data[i].a, &data[i].b, &data[i].c) == 4);

        res = res & (xRightRead(fp, &data[i].x1Right) == SUCCESS);
        res = res & (xRightRead(fp, &data[i].x2Right) == SUCCESS);

        res = res & (nRootsRightRead(fp, &data[i].nRootsRight) == SUCCESS);
    }

    return (res) ? SUCCESS : UNSUCCESS;
}

// reads right roots
enum result xRightRead(FILE *fp, double *xRight)
{
    if (!fscanf(fp, "%lf", xRight))
    {
        const int ST_NAN_LEN = 4;

        char stXRight[ST_NAN_LEN] = {};

        fscanf(fp, "%s", &stXRight);

        if (!strcmp(stXRight, "NAN"))
        {
            *xRight = NAN;

            return SUCCESS;
        }
        else
            return UNSUCCESS;
    }
    else
        return SUCCESS;
}

// reads right number of roots
enum result nRootsRightRead(FILE *fp, enum roots *nRootsRight)
{
    char stNRootsRight[MAX_STROOTS_LEN] = {};

    if (fscanf(fp, "%s", &stNRootsRight))
    {
        for (int i = 0; i < ROOTS_SIZE; i++)
        {
            if (!strcmp(stNRootsRight, stRoots[i]))
            {
                int numInRoots = i;

                switch (numInRoots)
                {
                    case NO_ROOTS :
                        *nRootsRight = NO_ROOTS;
                        break;
                    case ONE_ROOT :
                        *nRootsRight = ONE_ROOT;
                        break;
                    case TWO_ROOTS :
                        *nRootsRight = TWO_ROOTS;
                        break;
                    case INF_ROOTS :
                        *nRootsRight = INF_ROOTS;
                        break;
                    default:
                        return UNSUCCESS;
                }

                return SUCCESS;
            }
        }

        return UNSUCCESS;
    }

    return UNSUCCESS;
}

// n-�� ����
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

// ���������� ��� ����� ���� double
enum compareResult DoubleCompare(double num1, double num2)
{
    if (num1 - num2 > EPSILON)
        return FIRST_BIGGER;
    else if (num1 - num2 < -EPSILON)
        return SECOND_BIGGER;
    else
        return EQUAL;
}

// ������ ����� �������, ���� x1 > x2
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

// ������� ������
void BufferClear(void)
{
    while (getchar() != '\n');
}

// ������ ������
void ArrClear(char arr[], int arSize)
{
    for (int i = 0; i < arSize; i++)
        arr[i] = '\0';
}
