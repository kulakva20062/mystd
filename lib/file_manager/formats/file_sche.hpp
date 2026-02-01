//формат файла создал для личного проекта, не имеет ничего общего с какими-либо стандартами
#pragma once
#include <cstdint>
#include <cstddef>
#include "../regular_file.hpp"

class FileSche : public RegularFile {
    public:
        
    private:
        uint8_t size_x_; //Количество строк
        uint8_t size_y_; //Количество столбцов
        size_t size_z_; //Количество слоев

};