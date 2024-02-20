
#include <random>
#include <iostream>

class Vector {
    static constexpr double MIN_ANGLE = 0.0;
    static constexpr double MAX_ANGLE = 360.0;

    struct Bounds {
        double xMin, xMax;
        double yMin, yMax;
        double zMin, zMax;
    };

    public:
        double x, y, z;
        double pitch, yaw, roll;
        Bounds bounds = Vector::getBounds();

        Vector() {
            // Seed generator only once.
            m_generator.seed(m_rand());

            this->x = this->randomDouble(bounds.xMin, bounds.xMax);
            this->y = this->randomDouble(bounds.yMin, bounds.yMax);
            this->z = this->randomDouble(bounds.zMin, bounds.zMax);

            this->pitch = this->randomDouble(Vector::MIN_ANGLE, Vector::MAX_ANGLE);
            this->yaw = this->randomDouble(Vector::MIN_ANGLE, Vector::MAX_ANGLE);
            this->roll = this->randomDouble(Vector::MIN_ANGLE, Vector::MAX_ANGLE);
        }

    private:
        std::random_device m_rand;
        std::mt19937 m_generator;

        double randomDouble(double min, double max) {
            std::uniform_real_distribution<double> dd(min, max);
            return dd(m_generator);
        }

        Bounds getBounds() {
            /*  In here would you would use UE5 C++ API calls to get each of
                x, y and z boundaries of the world. Constants have been used
                since this function would be overridden. static constexpr not
                required here as these variables should not be encapsulated
                on the class.
            */ 
            double xMin = -10000.0, xMax = 10000.0;
            double yMin = -5000.0, yMax = 5000.0; 
            double zMin = 0.0, zMax = 10000.0;

            return Bounds{xMin, xMax, yMin, yMax, zMin, zMax};
        }
};

int main() {
    Vector v = Vector();
    /*  Here we just print the values for testing, but in UE5
        these values would be used to set the transform of the 
        BP_Actor with the CineCamera component. The Vector class
        would be replaced by the Transform class in UE5, but the 
        private randomDouble method gives an indication on how to
        achieve this, while maintaining code that compiles.
    */ 
    std::cout << "x=" << v.x << std::endl;
    std::cout << "y=" << v.y << std::endl;
    std::cout << "z=" << v.z << std::endl;

    std::cout << "pitch=" << v.pitch << std::endl;
    std::cout << "yaw=" << v.yaw << std::endl;
    std::cout << "roll=" << v.roll << std::endl;

    std::cout << "xMin=" << v.bounds.xMin << std::endl;
    std::cout << "xMMax=" << v.bounds.xMax << std::endl;
    
    std::cout << "yMin=" << v.bounds.yMin << std::endl;
    std::cout << "yMax=" << v.bounds.yMax << std::endl;

    std::cout << "zMin=" << v.bounds.zMin << std::endl;
    std::cout << "zMax=" << v.bounds.zMax << std::endl;
}