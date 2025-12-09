/*****************************************************************//**
 * \file   lab1.cpp
 * \brief  Лабораторная работа №1 по ООП
 * 
 * \author Viner Sakhibgareev
 * \date   November 2025
 *********************************************************************/


#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>
#include <mgl2/mgl.h>

using namespace std;

/**
 * \brief Структура точки в 3D пространстве.
 */
struct point3d
{
    double x, y, z;

    /**
     * \brief Конструктор по умолчанию.
     * 
     * \param x Координаты в плоскости x
     * \param y Координаты в плоскости y
     * \param z Координаты в плоскости z
     */
    point3d(double x = 0.0, double y = 0.0, double z = 0.0) : x(x), y(y), z(z) {}

    /**
     * \brief Функция вывода координат точки.
     */
    void print() const
    {
        cout << "Координаты точки: x = " << x << ", y = " << y << ", z = " << z << endl;
    }

    double getBackX() const { return x; }

    double getBackY() const { return y; }

    double getBackZ() const { return z; }
};

/**
 * \brief Класс для генерации случайных точек в правой половине тора.
 */
class TorusGenerator {
private:
    double R;
    double r;
    mt19937 gen;
    uniform_real_distribution<double> dist;

public:
    /**
     * \brief Конструктор с параметрами тора.
     * 
     * \param bigRadius Большой радиус (расстояние от центра тора до центра трубки)
     * \param smallRadius Малый радиус (радиус трубки)
     */
    TorusGenerator(double bigRadius = 3.0, double smallRadius = 1.0) : R(bigRadius), r(smallRadius), gen(random_device{}()), dist(0.0, 1.0) {}

    /**
     * \brief Функция генерации случайной точки внутри правой половины тора.
     * 
     * \return point3d(x, y, z) Данные точки по трем координатам
     */
    point3d rnd() {

        double u = dist(gen) * M_PI - M_PI / 2;
        double v = dist(gen) * 2 * M_PI;

        double surface_x = (R + r * cos(v)) * cos(u);
        double surface_y = (R + r * cos(v)) * sin(u);
        double surface_z = r * sin(v);

        double distance_to_move_inwards = dist(gen) * r;

        double dx = surface_x - R * cos(u);
        double dy = surface_y - R * sin(u);
        double length = sqrt(dx * dx + dy * dy);
        if (length > 1e-10) {
            dx /= length;
            dy /= length;
        }
        else {
            dx = cos(u);;
            dy = sin(u);
        }

        double x = surface_x - distance_to_move_inwards * dx;
        double y = surface_y - distance_to_move_inwards * dy;
        double z = surface_z;

        return point3d(x, y, z);
    }

    double getR() const { return R; }
    double getr() const { return r; }
};


/**
 * \brief Функция визуализции точек на 3D графике с помощью MathGL.
 * 
 * \param points Сгенерированные точки 
 * \param count Количество сгенерированных точек
 * \param R Большой радиус (расстояние от центра тора до центра трубки)
 * \param r Малый радиус (радиус трубки)
 */
void visualizePoints(point3d* points, int count, double R, double r)
{
    mglGraph gr;

    cout << "\nГенерация визуализации" << endl;

    // Настройки графика
    gr.SetSize(1200, 1000);
    gr.Rotate(50, 60);
    gr.Light(true);
    gr.Alpha(true);

    mglData xData(count);
    mglData yData(count);
    mglData zData(count);

    // Заполняем массивы координатами
    for (int i = 0; i < count; i++)
    {
        xData.a[i] = points[i].getBackX();
        yData.a[i] = points[i].getBackY();
        zData.a[i] = points[i].getBackZ();
    }

    // Рисуем оси координат
    gr.SetRanges(0, R + r, -(R + r), R + r, -r, r);
    gr.Axis();
    gr.Label('x', "X", 0);
    gr.Label('y', "Y", 0);
    gr.Label('z', "Z", 0);
    gr.Box();


    int nu = 30;  // Количество точек по углу u
    int nv = 20;  // Количество точек по углу v

    mglData torusX(nu, nv);
    mglData torusY(nu, nv);
    mglData torusZ(nu, nv);

    for (int i = 0; i < nu; i++)
    {
        double u = 2.0 * M_PI * i / (nu - 1);  // Угол от 0 до 2π

        // Только правая половина: u от -π/2 до π/2
        u = -M_PI / 2.0 + M_PI * i / (nu - 1);

        for (int j = 0; j < nv; j++)
        {
            double v = 2.0 * M_PI * j / (nv - 1);  // Угол от 0 до 2π

            // Параметрические уравнения тора
            torusX.a[i + nu * j] = (R + r * cos(v)) * cos(u);
            torusY.a[i + nu * j] = (R + r * cos(v)) * sin(u);
            torusZ.a[i + nu * j] = r * sin(v);
        }
    }

    gr.Surf(torusX, torusY, torusZ, "b8");  // Синий полупрозрачный
    gr.Mesh(torusX, torusY, torusZ, "k");   // Черная сетка

    gr.Plot(xData, yData, zData, "r. ");

    gr.WriteJPEG("C:\\Users\\Viner\\Desktop\\torus_visualization.jpg");
    cout << "Визуализация сохранена в файл: torus_visualization.jpg" << endl;

}


int main()
{
    setlocale(LC_ALL, "Russian");

    TorusGenerator generator;
    point3d* points = nullptr;
    int count = 0;

    cout << "Генератор точек в правой половине тора" << endl;
    cout << "Введите количество точек для генерации: ";
    cin >> count;

    points = new point3d[count];

    cout << "Генерация " << count << " точек" << endl;
    for (int i = 0; i < count; i++)
    {
        points[i] = generator.rnd();
    }
    cout << "Точки сгенерированы" << endl << endl;

    int choice;
    do
    {
        cout << "\nМеню" << endl;
        cout << "1. Вывести информацию о точке" << endl;
        cout << "2. Добавить новую точку вручную" << endl;
        cout << "3. Сохранить точки в файл points.txt" << endl;
        cout << "4. Визуализировать точки (MathGL)" << endl;
        cout << "0. Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;

        if (choice == 1)
        {
            int index;
            cout << "Введите номер точки (от 0 до " << count - 1 << "): ";
            cin >> index;

            if (index >= 0 && index < count)
            {
                cout << "\nТочка №" << index << ":" << endl;
                points[index].print();

                cout << "\nЧто вывести?" << endl;
                cout << "1. Только X" << endl;
                cout << "2. Только Y" << endl;
                cout << "3. Только Z" << endl;
                cout << "4. Ничего дополнительно" << endl;
                cout << "Выбор: ";
                int coordChoice;
                cin >> coordChoice;

                if (coordChoice == 1)
                    cout << "X = " << points[index].getBackX() << endl;
                else if (coordChoice == 2)
                    cout << "Y = " << points[index].getBackY() << endl;
                else if (coordChoice == 3)
                    cout << "Z = " << points[index].getBackZ() << endl;
            }
            else
            {
                cout << "Неверный номер точки!" << endl;
            }
        }
        else if (choice == 2)
        {
            double newX, newY, newZ;
            cout << "Введите координаты новой точки:" << endl;
            cout << "X: ";
            cin >> newX;
            cout << "Y: ";
            cin >> newY;
            cout << "Z: ";
            cin >> newZ;

            point3d* newPoints = new point3d[count + 1];
            for (int i = 0; i < count; i++)
            {
                newPoints[i] = points[i];
            }
            newPoints[count] = point3d(newX, newY, newZ);

            delete[] points;
            points = newPoints;
            count++;

            cout << "Точка добавлена. Всего точек: " << count << endl;
        }
        else if (choice == 3)
        {
            ofstream file("C:\\Users\\Viner\\Desktop\\points.txt");
            if (file.is_open())
            {
                for (int i = 0; i < count; i++)
                {
                    file << points[i].getBackX() << "  "
                        << points[i].getBackY() << "  "
                        << points[i].getBackZ() << endl;
                }
                file.close();
                cout << "Точки успешно сохранены в файл points.txt" << endl;
            }
            else
            {
                cout << "Ошибка при открытии файла!" << endl;
            }
        }
        else if (choice == 4)
        {
            visualizePoints(points, count, generator.getR(), generator.getr());
        }

    } while (choice != 0);

    delete[] points;
    return 0;
}