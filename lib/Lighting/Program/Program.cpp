#include "Program.h"

std::map<const char *, Lighting::Program *> Lighting::programs = std::map<const char *, Lighting::Program *>();

Lighting::Program::Program(const char *name,
                           uint32_t start_time,
                           uint32_t end_time,
                           Color color_start,
                           Color color_end)
{
    _start_time = Time(start_time*100).epochs();
    _end_time = Time(end_time*100).epochs();

    _color_start = color_start;
    _color_diff = color_end - color_start;

    std::memcpy(_name, name, 11);
    programs.insert(std::make_pair(name, this));
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
    uint32_t start_time = _start_time + offset;
    uint32_t end_time = _end_time + offset;

    float result = (timestamp - start_time);
    result = (result * 1.0) / (end_time - start_time);

    return result;
}

Lighting::Program *Lighting::getProgramToRun(uint32_t timestamp, uint32_t offset)
{
    for (const auto &program : programs)
    {
        bool is_executable = program.second->isExecutable(timestamp, offset);
        if (is_executable)
        {
            return program.second;
        }
    }
    return nullptr;
}
