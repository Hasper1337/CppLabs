//Лаба №1 9вар

#include <iostream>

struct point3d {
    private:
        double x, y, z;
    public:
        point3d() { x = 0.0; y = 0.0; z = 0.0; };

        double getBackX() const;
        double getBackY() const;
        double getBackZ() const;
        
        void set_coords(double x, double y, double z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        double get_x() {return this->x;}
        double get_y() {return this->y;}
        double get_z() {return this->z;}
}

class FillDots {
    private:
       double x, y, z; 
    public:

}
