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

const int SUB = 26;

const double EPSILON = 1e-6;

const int ROOTS_SIZE = 4;
const int MAX_STROOTS_LEN = 10;
const char stRoots[4][MAX_STROOTS_LEN] = {"NO_ROOTS", "ONE_ROOT", "TWO_ROOTS", "INF_ROOTS"};

enum testResult
{
    NO_ERRORS = 0,
    ONE_ERROR = 1
};
//���������� ���������� https://narodstream.ru/c-urok-19-modulnoe-programmirovanie-razdelnaya-kompilyaciya/
//���
enum roots
{
    NO_ROOTS  = 0,
    ONE_ROOT  = 1,
    TWO_ROOTS = 2,
    INF_ROOTS = 3
};

enum result
{
    SUCCESS        = 0,
    EOF_FOUND      = 1,
    FILE_READ_ER   = 2,
    FILE_NOT_FOUND = 3
};

enum yes_no
{
    NO  = 0,
    YES = 1
};

enum compareResult
{
    SECOND_BIGGER = -1,
    EQUAL         =  0,
    FIRST_BIGGER  =  1
};




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
enum result        SolverInput    (struct SolverParameters *par);
void               SolverOutput   (enum roots nRoots, double x1, double x2);
enum roots         Solve          (struct SolverParameters *par);
enum roots         LinearSolve    (struct SolverParameters *par);
enum roots         SquareSolve    (struct SolverParameters *par);

enum result        TestModule     (void);
enum result        UnitTester     (int *nErrors);
enum result        FileRead       (struct TestData data[], size_t arSize, FILE * fp);
enum result        xRightRead     (FILE *fp, double *xRight);
enum result        nRootsRightRead(FILE *fp, enum roots *nRootsRight);
enum testResult    Test           (struct TestData data);
void               SortRoots      (double* x1, double* x2);

void               ArrClear       (char arr[], int arSize);
enum result        Request        (enum yes_no * answer);

enum compareResult DoubleCompare  (double num1, double num2);

double             MinusZeroCheck (double num);

enum result        BufferClear    (void);

int main(void)
{
    enum result res = TestModule();

    if (res == SUCCESS)
        res = SolverModule();

    switch (res)
    {
        case EOF_FOUND :
            PRINTF_ITALICS("����� �����.\n");
            break;
        case FILE_READ_ER :
            PRINTF_RED_ITAL("������ �� ����� ������ �����.\n");
            break;
        case FILE_NOT_FOUND :
            PRINTF_RED_ITAL("���� �� ������.\n");
            break;
        case SUCCESS :
            break;
    }

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

            enum result res = UnitTester(&nErrors);

            if (res != SUCCESS)
                return res;
            else
                PRINTF_ITALICS("����� ������: %d\n", nErrors);
        }

        return SUCCESS;
    }
    else
        return EOF_FOUND;
}

// ����� ��������� � �������� ���������
enum result SolverModule(void)
{
    PRINTF_USUAL("� ����� ���������� ���������.\n");
    printf("��������� ����� ��� ax^2 + bx + c = 0\n");

    enum yes_no solvReq = YES;
    while (solvReq == YES)
    {
        if (SingleEquation() == EOF_FOUND)
            return EOF_FOUND;

        PRINTF_USUAL("������, ����� � ����� ��� ���� ���������?");

        if (Request(&solvReq) == EOF_FOUND)
            return EOF_FOUND;
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
        return EOF_FOUND;
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
    enum compareResult cmpRes = DoubleCompare(d, 0);

    switch (cmpRes)
    {
        case FIRST_BIGGER :
            (*par).x1 = min(MinusZeroCheck((-(*par).b - sqrt(d)) / (2 * (*par).a)),
                            MinusZeroCheck((-(*par).b + sqrt(d)) / (2 * (*par).a)));
            (*par).x2 = max(MinusZeroCheck((-(*par).b - sqrt(d)) / (2 * (*par).a)),
                            MinusZeroCheck((-(*par).b + sqrt(d)) / (2 * (*par).a)));

            return TWO_ROOTS;
        case EQUAL :
            (*par).x1 = MinusZeroCheck(-(*par).b / 2 / (*par).a);

            return ONE_ROOT;
        case SECOND_BIGGER :
            return NO_ROOTS;
    }
}

// ����
enum result SolverInput(struct SolverParameters *par)
{
    assert(par);

    PRINTF_USUAL("������� ���������� a, b � c ����� ������\n");
    PRINTF_VIOL_BACK_YEL_FORE();

    int EofCheck = 0;

    while ((EofCheck = scanf("%lf %lf %lf", &(*par).a, &(*par).b, &(*par).c)) != 3)
    {
        if (EofCheck == EOF)
            return EOF_FOUND;

        if (BufferClear() == EOF_FOUND)
            return EOF_FOUND;

        PRINTF_RED("������� ����� ���������\n");
        PRINTF_VIOL_BACK_YEL_FORE();
    }

    if (BufferClear() == EOF_FOUND)
        return EOF_FOUND;

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
            return EOF_FOUND;

        PRINTF_RED("����� ������� ������ \"��\" ��� \"���\".\n");
        PRINTF_VIOL_BACK_YEL_FORE();

        if (input[LAST_SYMB_OF_ANS] != '\0' && input[LAST_SYMB_OF_ANS] != '\n')
        {
            if (BufferClear() == EOF_FOUND)
                return EOF_FOUND;
        }

        ArrClear(input, SIZE_ANS);

        eofCheck = fgets(input, SIZE_ANS, stdin);
    }

    *answer = (!strcmp(input, "��\n")) ? YES : NO;

    return SUCCESS;
}

// ��������� Solve
enum result UnitTester(int *nErrors)
{
    assert(nErrors);

    FILE *fp = fopen("data.txt", "r");
    if (!fp)
        return FILE_NOT_FOUND;

    size_t arSize = 0;

    fscanf(fp, "%zd", &arSize);

    struct TestData tests[arSize];

    enum result res = FileRead(tests, arSize, fp);
    fclose(fp);

    if (res == SUCCESS)
    {
        for (size_t i = 0; i < arSize; i++)
            *nErrors += Test(tests[i]);

        return SUCCESS;
    }
    else
        return res;
}

// reads data in a file
enum result FileRead(struct TestData data[], size_t arSize, FILE *fp)
{
    assert(fp);

    for (size_t i = 0; i < arSize; i++)
    {
        int scRes = fscanf(fp, "%d %lf %lf %lf",
                           &data[i].nTest, &data[i].a, &data[i].b, &data[i].c);
        switch (scRes)
        {
            case EOF :
                return EOF_FOUND;
            case 4 :
                break;
            default:
                return FILE_READ_ER;
        }

        enum result res = xRightRead(fp, &data[i].x1Right);
        if (res != SUCCESS)
            return res;
        res = xRightRead(fp, &data[i].x2Right);
        if (res != SUCCESS)
            return res;

        res = nRootsRightRead(fp, &data[i].nRootsRight);
        if (res != SUCCESS)
            return res;
    }

    return SUCCESS;
}

// reads right roots
enum result xRightRead(FILE *fp, double *xRight)
{
    assert(fp);
    assert(xRight);

    int res = fscanf(fp, "%lf", xRight);

    switch (res)
    {
        case EOF :
            return EOF_FOUND;
        case 0 :
        {
            const int ST_NAN_LEN = 4;

            char stXRight[ST_NAN_LEN];

            fscanf(fp, "%s", &stXRight);

            if (!strcmp(stXRight, "NAN"))
            {
                *xRight = NAN;

                return SUCCESS;
            }
            else
                return FILE_READ_ER;
        }
        case 1 :
            return SUCCESS;
        default:
            printf("%d\n", res);
    }
}

// reads right number of roots
enum result nRootsRightRead(FILE *fp, enum roots *nRootsRight)
{
    assert(fp);
    assert(nRootsRight);

    char stNRootsRight[MAX_STROOTS_LEN] = {};

    int res = fscanf(fp, "%s", &stNRootsRight);

    switch (res)
    {
        case 1 :
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
                            return FILE_READ_ER;
                    }

                    return SUCCESS;
                }
            }

            return FILE_READ_ER;
        case EOF :
            return EOF_FOUND;
        case 0 :
            return FILE_READ_ER;
    }
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
enum result BufferClear(void)
{
    int c = 0;

    while (((c = getchar()) != '\n')  &&  (c != SUB) && (c != EOF))
        continue;

    if (c == EOF)
        return EOF_FOUND;
    else
        return SUCCESS;
}

// ������ ������
void ArrClear(char arr[], int arSize)
{
    for (int i = 0; i < arSize; i++)
        arr[i] = '\0';
}
