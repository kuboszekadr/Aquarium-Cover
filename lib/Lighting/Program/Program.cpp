#include "Program.h"

std::map<const char *, Lighting::Program*> Lighting::programs = std::map<const char*, Lighting::Program*>();

Lighting::Program::Program(const char *name,
                           uint32_t start_time,
                           uint32_t end_time,
                           Color color_start,
                           Color color_end)
{
    _start_time = start_time;
    _end_time = end_time;    
}

uint32_t Lighting::Program::getColor(uint32_t timestamp, uint32_t offset)
{
    float _progress = progress(timestamp, offset);

    return 0;
    // uint32_t r = (uint8_t)(_start[0] + _diff[0] * _progress);
    // uint32_t b = (uint8_t)(_start[1] + _diff[1] * _progress);
    // uint32_t w = (uint8_t)(_start[2] + _diff[2] * _progress);

    // uint32_t color = (b << 16) | (r << 8) | w;
    
    // return color;
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