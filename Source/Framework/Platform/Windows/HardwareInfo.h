#pragma once

namespace GameDev2D
{
	class HardwareInfo
	{
    public:
        HardwareInfo() = default;
        ~HardwareInfo() = default;

        //Returns the number of available CPUs on the machine
        unsigned int CpuCount();

        //Returns how much memory (RAM) is installed on the machine
        unsigned long long MemoryInstalled();

        //Hard drive specific methods
        unsigned long long DiskSpaceUsed();
        unsigned long long DiskSpaceFree();
        unsigned long long DiskSpaceTotal();

        //Logs platform specific details about OS, CPU, RAM, HDD, GPU
        void Log();
	};
}