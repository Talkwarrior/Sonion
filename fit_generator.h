#pragma once
#include <fstream>
#include <qlist.h>

#define MATRIX_MAX_SIZE 200
#define PART_LEN 5

typedef struct point
{
    float x;
    float y;
} Point;

typedef struct matrix
{
    int size;
    float field[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];
} matrix;

void fit_generator(const QList<QPointF>& data, std::ofstream& out);