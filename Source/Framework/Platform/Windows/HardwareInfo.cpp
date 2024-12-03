#include "HardwareInfo.h"
#include "../Platform.h"
#include "../../Debug/Log.h"


namespace GameDev2D
{
	unsigned int HardwareInfo::CpuCount()
	{
		unsigned int cpuCount = 0;
#if GAMEDEV2D_OS_WINDOWS
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		cpuCount = sysinfo.dwNumberOfProcessors;
#endif
		return cpuCount;
	}

	unsigned long long HardwareInfo::MemoryInstalled()
	{
		unsigned long long physicalMemory = 0;
#if GAMEDEV2D_OS_WINDOWS
		GetPhysicallyInstalledSystemMemory(&physicalMemory);
		physicalMemory *= 1024;
#endif
		return physicalMemory;
	}

	unsigned long long HardwareInfo::DiskSpaceUsed()
	{
		return DiskSpaceTotal() - DiskSpaceFree();
	}

	unsigned long long HardwareInfo::DiskSpaceFree()
	{
		unsigned long long lpFreeBytesAvailableToCaller, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes = 0;
#if GAMEDEV2D_OS_WINDOWS
		GetDiskFreeSpaceExA("C:", (PULARGE_INTEGER)&lpFreeBytesAvailableToCaller, (PULARGE_INTEGER)&lpTotalNumberOfBytes, (PULARGE_INTEGER)&lpTotalNumberOfFreeBytes);
#endif
		return lpTotalNumberOfFreeBytes;
	}

	unsigned long long HardwareInfo::DiskSpaceTotal()
	{
		unsigned long long lpFreeBytesAvailableToCaller, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes = 0;
#if GAMEDEV2D_OS_WINDOWS
		GetDiskFreeSpaceExA("C:", (PULARGE_INTEGER)&lpFreeBytesAvailableToCaller, (PULARGE_INTEGER)&lpTotalNumberOfBytes, (PULARGE_INTEGER)&lpTotalNumberOfFreeBytes);
#endif
		return lpTotalNumberOfBytes;
	}

	void HardwareInfo::Log()
	{
		Log::Message(LogVerbosity::Hardware, "[HardwareInfo] CPU cores: %u", CpuCount());
		Log::Message(LogVerbosity::Hardware, "[HardwareInfo] Memory installed: %.2f-GB", ((float)MemoryInstalled() / 1024.0f / 1024.0f / 1024.0f));
		Log::Message(LogVerbosity::Hardware, "[HardwareInfo] Disk space total: %.2f-GB", ((float)DiskSpaceTotal() / 1024.0f / 1024.0f / 1024.0f));
		Log::Message(LogVerbosity::Hardware, "[HardwareInfo] Disk space used: %.2f-GB", ((float)DiskSpaceUsed() / 1024.0f / 1024.0f / 1024.0f));
		Log::Message(LogVerbosity::Hardware, "[HardwareInfo] Disk space free: %.2f-GB", ((float)DiskSpaceFree() / 1024.0f / 1024.0f / 1024.0f));
	}
}