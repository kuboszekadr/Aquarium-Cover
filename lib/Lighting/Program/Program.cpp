#include "Program.h"

uint8_t Lighting::programs_amount = 0;
Lighting::Program *Lighting::programs[10];

Lighting::Program::Program(const char *name,
                           Time start_time,
                           Time end_time,
                           Color color_start,
                           Color color_end)
{
    _start_time = start_time.toMillis();
    _end_time = end_time.toMillis();

    _color_start = color_start;
    _color_diff = color_end - color_start;

    memset(_name, 0 ,11);
    std::memcpy(_name, name, 11);

    programs[programs_amount] = this;
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
    // FIXME: modify timestamp instaed of start/end time
    uint32_t start_time = _start_time + offset;
    uint32_t end_time = _end_time + offset;

    float result = (timestamp - start_time);
    result = (result * 1.0) / (end_time - start_time);

    return result;
}

Lighting::Program *Lighting::getProgramToRun(uint32_t timestamp, uint32_t offset)
{
    for (int i = 0; i < programs_amount; i++)
    {
        Program *program = programs[i];
        bool is_executable = program->isExecutable(timestamp, offset);
        
        if (is_executable)
        {
            return program;
        }        
    }
    return nullptr;
}
