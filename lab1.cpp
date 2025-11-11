#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

// Структура
struct point3d
{
    double x, y, z;

    point3d(double x = 0.0, double y = 0.0, double z = 0.0)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void print() const
    {
        cout << "Координаты точки: x = " << x << ", y = " << y << ", z = " << z << endl;
    }

    double getBackX() const
    {
        return x;
    }

    double getBackY() const
    {
        return y;
    }

    double getBackZ() const
    {
        return z;
    }
};

// Класс для генерации случайных точек в правой половине тора
class TorusGenerator{
    double R; // Большой радиус тора
    double r; // Малый радиус тора

public:
    TorusGenerator(double bigRadius = 3.0, double smallRadius = 1.0)
    {
        R = bigRadius;
        r = smallRadius;
        srand(time(0));
    }

    // Функция для генерации случайной точки внутри правой половины тора
    point3d rnd()
    {
        double x, y, z;
        bool inside = false;

        while (!inside)
        {
            x = ((double)rand() / RAND_MAX) * (R + r);
            y = ((double)rand() / RAND_MAX) * 2.0 * (R + r) - (R + r);
            z = ((double)rand() / RAND_MAX) * 2.0 * r - r;

            double distanceFromCenter = sqrt(x * x + y * y);
            double distanceFromTube = sqrt((distanceFromCenter - R) * (distanceFromCenter - R) + z * z);

            if (distanceFromTube <= r && x >= 0)
            {
                inside = true;
            }
        }

        return point3d(x, y, z);
    }

    double getR() const { return R; }
    double getr() const { return r; }
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

    cout << "До свидания!" << endl;
    return 0;
}