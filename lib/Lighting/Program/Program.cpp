#include "Program.h"

uint8_t Lighting::programs_amount = 0;
Lighting::Program *Lighting::programs[10];

Lighting::Program::Program(const char *name,
                           uint32_t start_time,
                           uint32_t end_time,
                           Color color_start,
                           Color color_end)
{
    uint8_t start_hour = start_time / 100;
    uint8_t start_minute = start_time - start_hour*100;
    
    _start_time = Time(start_hour, start_minute, 0).epochs();
    _end_time = Time(end_time*100).epochs();

    _color_start = color_start;
    _color_diff = color_end - color_start;

    memset(_name, 0 ,11);
    std::memcpy(_name, name, 11);

    programs[programs_amount] = this; // add sensor to the list of sensors
    programs_amount++;
}

uint32_t Lighting::Program::getColor(uint32_t timestamp, uint32_t offset)
{
    float _progress = progress(timestamp, offset);
    Color color = _color_start + _color_diff * _progress;

    uint32_t result = color.toPixelColor();
    return result;
}

bool Lighting::Program::isExecutable(uint32_t timestamp, uint32_t offset)
{
    uint32_t start_time = _start_time + offset;
    uint32_t end_time = _end_time + offset;

    bool result = ((timestamp >= start_time) & (timestamp <= end_time));
    return result;
}

float Lighting::Program::progress(uint32_t timestamp, uint32_t offset)
{
    uint32_t start_time = _start_time;
    uint32_t end_time = _end_time;

    float result = (timestamp - start_time);
    result = (result * 1.0) / (end_time - start_time);

    return result;
}

Lighting::Program *Lighting::getProgramToRun(uint32_t timestamp, uint32_t offset)
{
    for (int i = 0; i < programs_amount; i++)
    {
        Program *program = programs[i];
        bool is_executable = program->isExecutable(timestamp, 0);
        
        if (is_executable)
        {
            return program;
        }        
    }
    return nullptr;
}
