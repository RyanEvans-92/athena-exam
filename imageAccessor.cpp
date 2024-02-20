#include <iostream>
#include <map>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>
#include <random>

class Image {
    public:
        // Static variables here, but would be useful to have this set
        // in the constructor to support images of varying size.
        static constexpr int WIDTH = 1920;
        static constexpr int HEIGHT = 1080;
        // Struct for an RGB pixel values. Values are between 0-255.
        struct Pixel_RGB { unsigned char r, g, b; };

        /* Generate a flattened image of the size WIDTH*HEIGHT. This function
            would be replaced with an image import function that takes camera
            snapshots from UE5.
        */
        static Pixel_RGB* generateImage() {
            Pixel_RGB* data = new Pixel_RGB[Image::size()];

            for (int i = 0; i < Image::size(); i++) {
                // Not random values, but close enough for our purposes.
                data[i] = Pixel_RGB{static_cast<unsigned char>(10 * i % 255), 
                                    static_cast<unsigned char>(12 * i % 255), 
                                    static_cast<unsigned char>(50 * i % 255)};
            }

            return data;
        }

        // Testing function only. Only use to check data is being stored in
        // an expected way.
        void printData() {
            for (auto i = 0; i < Image::size(); i++) {
                std::cout << i << ": " << '(' << static_cast<int>(this->m_data[i].r) << ',' <<
                    static_cast<int>(this->m_data[i].g) << ',' <<
                    static_cast<int>(this->m_data[i].b) << ')' << std::endl;
            }
        }

        static constexpr long size() { return Image::WIDTH * Image::HEIGHT; }

        Image(Pixel_RGB* data) : m_data(data) {}

        ~Image() { delete[] m_data; }
        
        Image(const Image& other) : m_data(new Pixel_RGB[size()]) {
            std::copy(other.m_data, other.m_data + size(), m_data);
        }

        // Copy assignment operator
        Image& operator=(const Image& other) {
            if (this != &other) {
                delete[] m_data;
                m_data = new Pixel_RGB[size()];
                std::copy(other.m_data, other.m_data + size(), m_data);
            }
            return *this;
        }

        private:
            Image::Pixel_RGB* m_data;
};

class Accessor_ThreadSafe {
    static constexpr int MAP_SIZE_LIMIT = 10;

    private:
        std::map<int, Image> m_imageMap;
        std::mutex m_mut;
        std::random_device m_rand;
        std::mt19937 m_generator;

    public:
        void insert(int key, const Image& img) {
            std::lock_guard<std::mutex> lock(m_mut);
            
            // Implement FIFO data management when limiting the number of images stored.
            while (m_imageMap.size() >= Accessor_ThreadSafe::MAP_SIZE_LIMIT) { 
                m_imageMap.erase(m_imageMap.begin());
            }

            m_imageMap.insert(std::make_pair(key, img));
        }

        Image get(int key) {
            std::lock_guard<std::mutex> lock(m_mut);
            auto it = m_imageMap.find(key);
            return (it != m_imageMap.end()) ? it->second : nullptr;
        }

        void processImage(int key) {
            /*  Seed generator every time since constructor is otherwise
                not needed. The purpose of this is to generate a random
                time delay to simulate processing, so the additional 
                overhead is irrelevant.
            */
            m_generator.seed(m_rand());
            std::uniform_int_distribution<int> d(0, 1000);
            int delay = d(m_generator);

            std::lock_guard<std::mutex> lock(m_mut);
            std::cout << "----------------------" << std::endl;
            std::cout << "Processing image: " << key << std::endl;
            std::cout << "Current map size: " << m_imageMap.size() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            std::cout << "Image " << key << " processed in " << delay << " ms." << std::endl;
            std::cout << "----------------------" << std::endl;
        }
};

int main() {
    constexpr int numImgs = 1000;

    Accessor_ThreadSafe accessor;
    std::vector<std::thread> threads;

    for (int i = 0; i <= numImgs; i++) {
        threads.emplace_back([i, &accessor]() {
            accessor.insert(i, Image(Image::generateImage()));
            accessor.processImage(i);
        });
    }

    for (auto& th:threads) { th.join(); }

    return 0;
}