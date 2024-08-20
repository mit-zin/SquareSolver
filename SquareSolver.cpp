#include <TXLib.h>
#include <stdio.h>
#include <math.h>

int solver(double a, double b, double c, double* x1, double* x2);
int SquareSolver(double a, double b, double c, double* x1, double* x2);
int NotSquareSolver(double b, double c, double* x);
void entering(double* a, double* b, double* c);
void ans(int rootsn, double x1, double x2);
double MinusNullCheck(double num);

const int SS_ENF = -1;
const double EPSILON = 0.000001;

int main(void)
{
    double a = NAN, b = NAN, c = NAN;

    entering(&a, &b, &c);

    double x1 = NAN, x2 = NAN;
    int rootsn = 0;

    rootsn = solver(a, b, c, &x1, &x2);

    ans(rootsn, x1, x2);

    return 0;
}

int solver(double a, double b, double c, double* x1, double* x2)    //������ ���������
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

int NotSquareSolver(double b, double c, double* x)  //������ ���� ��������� a ����� 0
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

int SquareSolver(double a, double b, double c, double* x1, double* x2)  //������ ���� ��������� a �� ����� 0
{
    double d = b * b - 4 * a * c;

    if (d > 0)
    {
        *x1 = MinusNullCheck((-b - sqrt(d)) / 2 / a);
        *x2 = MinusNullCheck((-b + sqrt(d)) / 2 / a);
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

void entering(double* a, double* b, double* c)  //����
{
    printf("� ����� ���������� ���������.\n");
    printf("��������� ����� ��� ax^2 + bx + c = 0\n");
    printf("������� ���������� a, b � c ����� ������\n");

    int ent = 0;
    ent = scanf("%lf %lf %lf", a, b, c);
    while (ent != 3)
    {
        printf("������� ����� ���������\n");
        ent = scanf("%*s %lf %lf %lf", a, b, c);
    }
}

void ans(int rootsn, double x1, double x2)    //�����
{
    switch (rootsn)
    {
        case 0 :
            printf("��������� �� ����� ������.");
            break;
        case 1 :
            if (fabs(x1) < EPSILON)
            {
                x1 = 0.0;
            }
            printf("��������� ����� ���� ������: %g", x1);
            break;
        case 2 :
            printf("��������� ����� ��� �����: %g � %g", x1, x2);
            break;
        case SS_ENF :
            printf("��������� ����� ����������� ���������� ������");
            break;
        default :
            printf("error");
    }
}

double MinusNullCheck(double num)   //������ -0 �� 0
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
