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

        double inner_radius = dist(gen) * r;
        double sign = (dist(gen) < 0.5) ? -1.0 : 1.0;
        double offset = sign * inner_radius;

        double dx = surface_x - R * cos(u);
        double dy = surface_y - R * sin(u);
        double length = sqrt(dx * dx + dy * dy);
        if (length > 1e-10) {
            dx /= length;
            dy /= length;
        }
        else {
            dx = 1.0;
            dy = 0.0;
        }

        double x = surface_x + offset * dx;
        double y = surface_y + offset * dy;
        double z = surface_z;

        return point3d(x, y, z);
    }
};


int main()
{
    setlocale(LC_ALL, "Russian");

    TorusGenerator generator;
    point3d* points = nullptr;
    int count = 0;

    cout << "=== Генератор точек в правой половине тора ===" << endl;
    cout << "Введите количество точек для генерации: ";
    cin >> count;

    // массив точек
    points = new point3d[count];

    // заполнение массива случайными точками
    cout << "Генерация " << count << " точек..." << endl;
    for (int i = 0; i < count; i++)
    {
        points[i] = generator.rnd();
    }
    cout << "Точки успешно сгенерированы!" << endl << endl;

    int choice;
    do
    {
        cout << "\n=== Меню ===" << endl;
        cout << "1. Вывести информацию о точке" << endl;
        cout << "2. Добавить новую точку вручную" << endl;
        cout << "3. Сохранить точки в файл points.txt" << endl;
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

            cout << "Точка добавлена! Теперь всего точек: " << count << endl;
        }
        else if (choice == 3)
        {
            ofstream file("points.txt");
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

    } while (choice != 0);

    // Освобождаем память
    delete[] points;
    return 0;
}