#ifndef VECTOR_3D
#define VECTOR_3D
class Vector3D {

  private:
    double x_coordinate;
    double y_coordinate;
    double z_coordinate;

  public:

    Vector3D() {
        x_coordinate = 0;
        y_coordinate = 0;
        z_coordinate = 0;
    }
    
    void set_x_cord(double x) {
      x_coordinate = x;
    }

    void set_y_cord(double y) {
      y_coordinate = y;
    }

    void set_z_cord(double z) {
      z_coordinate = z;
    }

    double get_x_cord(void) {
        return x_coordinate;
    }

    double get_y_cord(void) {
        return y_coordinate;
    }

    double get_z_cord(void) {
        return z_coordinate;
    }
};
#endif