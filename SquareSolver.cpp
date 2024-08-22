#include <TXLib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

int solver(double a, double b, double c, double* x1, double* x2);
int SquareSolver(double a, double b, double c, double* x1, double* x2);
int LinearSolver(double b, double c, double* x);
void input(double* a, double* b, double* c);
void ans(int rootsn, double x1, double x2);
double MinusNullCheck(double num);
int tester(void);
int nTester(int nTest, double a, double b, double c, double x1Right, double x2Right, int rootsn);
bool DoubleZeroCheck(double num);
void SortRoots(double* x1, double* x2);
void bufferClear(void);

//������ �����, ��������� ������� ������ � ���������

const double EPSILON = 0.000001;
const int SUCCESS_END = 0;
const int NO_ROOTS = 0;
const int ONE_ROOT = 1;
const int TWO_ROOTS = 2;
const int INF_ROOTS = -1;
const int ONE_ERROR = 1;
const int NO_ERRORS = 0;

int main(void)
{
    int nErrors = tester();
    printf("����� ������: %d\n", nErrors);

    double a = NAN, b = NAN, c = NAN;

    input(&a, &b, &c);

    double x1 = NAN, x2 = NAN;
    int rootsn = 0;

    rootsn = solver(a, b, c, &x1, &x2);

    ans(rootsn, x1, x2);

    return SUCCESS_END;
}

int solver(double a, double b, double c, double* x1, double* x2)    //������ ���������
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

int LinearSolver(double b, double c, double* x)  //������ ���� ��������� a ����� 0
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

int SquareSolver(double a, double b, double c, double* x1, double* x2)  //������ ���� ��������� a �� ����� 0
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

void input(double* a, double* b, double* c)  //����
{
    assert(a);
    assert(b);
    assert(c);

    printf("� ����� ���������� ���������.\n");
    printf("��������� ����� ��� ax^2 + bx + c = 0\n");
    printf("������� ���������� a, b � c ����� ������\n");

    while (scanf("%lf %lf %lf", a, b, c) != 3)
    {
        bufferClear();                         //TODO: ������� ������� �������
            // EOF
        printf("������� ����� ���������\n");
    }
}

void ans(int rootsn, double x1, double x2)    //�����
{
    switch (rootsn)
    {
        case NO_ROOTS :
            printf("��������� �� ����� ������.");
            break;
        case ONE_ROOT :
            printf("��������� ����� ���� ������: %g", x1);
            break;
        case TWO_ROOTS :
            printf("��������� ����� ��� �����: %g � %g", x1, x2);
            break;
        case INF_ROOTS :
            printf("��������� ����� ����������� ���������� ������");
            break;
        default :
            printf("error");
    }
}

double MinusNullCheck(double num)   //������ -0 �� 0
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

int tester(void)    //��������� solver()
{
    int nErrors = 0;

    nErrors += nTester(1, 0.0, 0.0, 0.0, NAN, NAN, INF_ROOTS);
    nErrors += nTester(2, 0.0, 0.0, 1.0, NAN, NAN, NO_ROOTS);
    nErrors += nTester(3, 0.0, 2.0, 0.0, 0.0, NAN, ONE_ROOT);
    nErrors += nTester(4, 0.0, 2.0, 3.0, -1.5, NAN, ONE_ROOT);
    nErrors += nTester(5, 0.0, 1.0, -5.0, 5.0, NAN, ONE_ROOT);
    nErrors += nTester(6, 0.0, 1.5, 2.25, -1.5, NAN, ONE_ROOT);
    nErrors += nTester(7, 2.0, 0.0, 0.0, 0.0, NAN, ONE_ROOT);
    nErrors += nTester(8, -3.0, 0.0, 27.0, 3.0, -3.0, TWO_ROOTS);
    nErrors += nTester(9, 1.0, 0.0, 4.0, NAN, NAN, NO_ROOTS);
    nErrors += nTester(10, 0.7, 0.525, -1.75, -2.0, 1.25, TWO_ROOTS);
    nErrors += nTester(11, 3.0, -6.0, 3.0, 1.0, NAN, ONE_ROOT);

    return nErrors;
}

int nTester(int nTest, double a, double b, double c, double x1Right, double x2Right, int rootsnRight)   //n-�� ����
{
    double x1 = NAN, x2 = NAN;
    int rootsn = solver(a, b, c, &x1, &x2);

    SortRoots(&x1Right, &x2Right);

    if (rootsn != rootsnRight || fabs(x1 - x1Right) > EPSILON || fabs(x2 - x2Right) > EPSILON)
    {
        printf("Error Test %d: a = %lg, b = %lg, c = %lg, x1 = %lg, x2 = %lg, rootsn = %d\n"
               "Expected: x1 = %lg, x2 = %lg, rootsn = %d\n",
               nTest, a, b, c, x1, x2, rootsn, x1Right, x2Right, rootsnRight);

        return ONE_ERROR;
    }
    else
    {
        return NO_ERRORS;
    }
}

bool DoubleZeroCheck(double num)    //���������, ��� ���������� ���� double ����� ����
{
    return fabs(num) < EPSILON;
}

void SortRoots(double* x1, double* x2)  //������ ����� �������, ���� x1 > x2
{
    if (*x1 > *x2)
    {
        double change = *x1;
        *x1 = *x2;
        *x2 = change;
    }
}

void bufferClear(void)
{
    while (getchar() != '\n');
}
