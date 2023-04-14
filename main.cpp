#include <iostream>
#include <time.h>
#include <omp.h>
#include <vector>
#include <chrono>
using namespace std;

bool search_reverse_matrix(vector <vector<double>> &matrix)
{
    int size = matrix.size();
    vector <vector<double>> E(size, vector<double>(size));

    //Заполнение единичной матрицы
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i == j) E[i][j] = 1.0;
            else E[i][j] = 0.0;
        }
    }

    //Трансформация исходной матрицы в верхнетреугольную
    for (int k = 0; k < size; k++)
    {
        if (abs(matrix[k][k]) < 1e-8)
        {
            bool changed = false;

            for (int i = k + 1; i < size; i++)
            {
                if (abs(matrix[i][k]) > 1e-8)
                {
                    swap(matrix[k], matrix[i]);
                    swap(E[k], E[i]);
                    changed = true;
                    break;
                }
            }

            if (!changed)
                return false;
        }

        double div = matrix[k][k];

        #pragma omp parallel
        {
            #pragma omp for
            for (int j = 0; j < size; j++)
            {
                matrix[k][j] /= div;
                E[k][j] /= div;
            }
        }

        #pragma omp parallel
        {
            #pragma omp for
            for (int i = k + 1; i < size; i++)
            {
                double multi = matrix[i][k];


                for (int j = 0; j < size; j++)
                {
                    matrix[i][j] -= multi * matrix[k][j];
                    E[i][j] -= multi * E[k][j];
                }
            }
        }
    }

    //Формирование единичной матрицы из исходной
    //и обратной из единичной
    for (int k = size - 1; k > 0; k--)
    {
        #pragma omp parallel
        {
            #pragma omp for
            for (int i = k - 1; i > -1; i--)
            {
                double multi = matrix[i][k];

                for (int j = 0; j < size; j++)
                {
                    matrix[i][j] -= multi * matrix[k][j];
                    E[i][j] -= multi * E[k][j];
                }
            }
        }
    }
    matrix = E;
    return true;
}

double random(const int min, const int max)
{
    if (min == max)
        return min;
    return min + rand() % (max - min);
}

int main() {
    setlocale(LC_ALL, "RUS");
    int equations_amount;

    for (int num_threads = 1; num_threads <= 1; num_threads++) {
        omp_set_num_threads(num_threads);
        for (int z = 1; z <= 10; z++) {
            equations_amount = 500*z;
            omp_set_num_threads(num_threads);
            vector<vector<double>> matrix(equations_amount, vector<double>(equations_amount));
            vector<double> B(equations_amount);

            // Заполняем матрицу коэффициентов и B
            for (int i = 0; i < equations_amount; i++) {
                for (int j = 0; j < equations_amount; j++)
                    matrix[i][j] = random(0, 100);
                B[i] = random(0, 100);
            }

            auto start = std::chrono::high_resolution_clock::now();

            // Вычисление обратной матрицы
            if (!search_reverse_matrix(matrix)) {
                cout << "\nНевозможно найти обратную матрицу!\n";
                exit(1);
            }

            // Матрица-столбец неизвестных X и вычисление окончательного результата
            vector<double> X(equations_amount);
            #pragma omp parallel
            {
                #pragma omp for
                for (int i = 0; i < equations_amount; i++) {
                    X[i] = 0;
                    for (int j = 0; j < equations_amount; j++)
                        X[i] += matrix[i][j] * B[j];
                }
            }

            auto end = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << "Время выполнения: " << duration.count() / 10 << " мс." << "Потоки: " << num_threads
                      << "Уравнений: " << equations_amount << std::endl;
        }
    }
    return 0;
}