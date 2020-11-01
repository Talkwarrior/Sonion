#include "fit_generator.h"
#include <qmessagebox.h>

matrix coef;

// inverse function for square matrix(equation)
static void inverse(matrix* equation)
{
    const int size = equation->size;

    matrix* inv = new matrix;
    inv->size = size;

    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            inv->field[row][col] = 0;
        }
        inv->field[row][row] = 1;
    }

    for (int rows = 0; rows < size; rows++)
    {
        // divide whole row with equation.field[rows][rows]
        if (equation->field[rows][rows] == 0)
        {
            // divide by zero
            QMessageBox::warning(nullptr, "sorry", "divide by zero");
        }

        const float b = equation->field[rows][rows];
        for (int c = 0; c < size; c++)
        {
            equation->field[rows][c] /= b;
            inv->field[rows][c] /= b;
        }

        int r = rows;
        for (int i = 0; i < size - 1; i++)
        {
            r = (r + 1) % size;
            const float t = equation->field[r][rows];

            for (int c = 0; c < size; c++)
            {
                inv->field[r][c] -= t * inv->field[rows][c];
                equation->field[r][c] -= t * equation->field[rows][c];
            }
        }
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            equation->field[i][j] = inv->field[i][j];
        }
    }
    delete inv;
}

// multiple of 2-Dimension Matrix, eq * res
static void matrix_multiple(matrix* equation, matrix* result)
{
    coef.size = result->size;

    for (int row = 0; row < coef.size; row++)
    {
        for (int column = 0; column < coef.size; column++)
        {
            // fill coef[row][column] by scalar multiple
            coef.field[row][column] = 0;
            for (int e = 0; e < equation->size; e++)
            {
                coef.field[row][column] += equation->field[row][e] * result->field[e][column];
            }
        }
    }
}

// returns expression
static std::string expression;
static int perfect_fit(const QList<QPointF>& data, const int begin, const int end)
{
    const int size = end - begin + 1;
    if (size == 1) { return 1; }

    matrix* equation = new matrix();
    equation->size = size;

    for (int row = 0; row < size; row++)
    {
        float p = 1.0;
        float x = data.at(row + begin).x();
        for (int cols = size - 1; cols >= 0; cols--)
        {
            equation->field[row][cols] = p;
            p = x * p;
        }
    }
    inverse(equation);

    // create y-vector(the matrix with results)
    matrix* result = new matrix();
    result->size = size;
    result->size = 1;
    for (int row = 0; row < size; row++)
    {
        result->field[row][0] = data.at(row + begin).y();
    }
    matrix_multiple(equation, result);

    // init expression
    expression[0] = '\0';
    std::string f = "";
    for (int i = 0; i < size; i++)
    {
        f += std::to_string(coef.field[i][0]);
        f += "*x**"; 
        f += std::to_string(size - i - 1);
        expression += f;
    }

    return 0;
}

void fit_generator(const QList<QPointF>& data, std::ofstream& out)
{
    const int count = data.length();
    int partCnt = count / (PART_LEN - 1) + (count % (PART_LEN - 1) > 0);
    int begin = 0;
    int end = PART_LEN - 1;

    for (int part = 0; part < partCnt; part++)
    {
        if (perfect_fit(data, begin, end))
        {
            // failed to fit
            partCnt--;
            break;
        }
        out << "f" << part << "(x)=" << expression << '\n';
        expression = "";
        begin = end;
        end = (end + PART_LEN - 1) > count - 1 ? count - 1 : end + PART_LEN - 1;
    }
    begin = 0;
    end = PART_LEN - 1;
    for (int part = 0; part < partCnt; part++)
    {
        if (part != partCnt)
        {
            out << "+";
        }
        out << "f" << part << "(x)*(x>=" << data.at(begin).x() << ")*(x<" << data.at(end).x() << ")";
        begin = end;
        end = (end + PART_LEN - 1) > count - 1 ? count - 1 : end + PART_LEN - 1;
    }
}
