#ifndef Lighting_Program_h
#define Lighting_Program_h

#include <map>
#include <array>

namespace Lighting
{

    class Program
    {
    public:
        Program(const char *name,
                uint32_t start_time,
                uint32_t end_time,
                std::array<int, 3> &start,
                std::array<int, 3> &end);

        uint32_t getColor(uint32_t timestamp, uint32_t offset);

        bool isExecutable(uint32_t timestamp, uint32_t offset);
        float progress(uint32_t timestamp, uint32_t offset);

    private:
        uint32_t _start_time;
        uint32_t _end_time;

        std::array<int, 3> _start;
        std::array<int, 3> _diff;
    };
    extern std::map<const char *, Program *> programs;

}

#endif