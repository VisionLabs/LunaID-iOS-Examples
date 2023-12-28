#pragma once

#if defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || defined(_M_IX86)
	#define CPU_ARCH_X86 1
#endif

#if defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
	#define CPU_ARCH_X86_64 1
#endif

#if defined(__arm__) || defined(_M_ARM)
	#define CPU_ARCH_ARM 1
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
	#define CPU_ARCH_ARM64 1
#endif

#if defined(EMSCRIPTEN)
	#define CPU_ARCH_ASMJS 1
#endif

namespace vlc
{
	class ICPUInfo
	{
	public:
		ICPUInfo() = default;
		virtual ~ICPUInfo() = default;

		ICPUInfo(const ICPUInfo&) = delete;
		ICPUInfo& operator = (const ICPUInfo&) = delete;

		virtual const char* vendorName() const = 0;
		virtual const char* cpuName() const = 0;

		virtual bool isX86() const = 0;
		virtual bool isARM() const = 0;

		virtual bool hasSSE2() const = 0;
		virtual bool hasSSE3() const = 0;
		virtual bool hasSSSE3() const = 0;
		virtual bool hasSSE41() const = 0;
		virtual bool hasSSE42() const = 0;
		virtual bool hasAVX() const = 0;
		virtual bool hasAVX2() const = 0;
		virtual bool hasFMA() const = 0;
		virtual bool hasNEON() const = 0;

		virtual bool hasAVX512F() const = 0;
		virtual bool hasAVX512DQ() const = 0;
		virtual bool hasAVX512VL() const = 0;
		virtual bool hasAVX512IFMA() const = 0;
		virtual bool hasAVX512VBMI() const = 0;
		virtual bool hasAVX512VBMI2() const = 0;
		virtual bool hasAVX512VNNI() const = 0;

		virtual uint32_t L2CacheLineSize() const = 0;
		virtual uint32_t L2CacheTotalSize() const = 0;
	};

	class CPUInfo : public ICPUInfo
	{
	public:
		CPUInfo();
		~CPUInfo();

		CPUInfo(const CPUInfo&) = delete;
		CPUInfo& operator = (const CPUInfo&) = delete;

		const char* vendorName() const override;
		const char* cpuName() const override;

		bool isX86() const override;
		bool isARM() const override;

		bool hasSSE2() const override;
		bool hasSSE3() const override;
		bool hasSSSE3() const override;
		bool hasSSE41() const override;
		bool hasSSE42() const override;
		bool hasAVX() const override;
		bool hasAVX2() const override;
		bool hasFMA() const override;
		bool hasNEON() const override;

		bool hasAVX512F() const override;
		bool hasAVX512DQ() const override;
		bool hasAVX512VL() const override;
		bool hasAVX512IFMA() const override;
		bool hasAVX512VBMI() const override;
		bool hasAVX512VBMI2() const override;
		bool hasAVX512VNNI() const override;

		uint32_t L2CacheLineSize() const override;
		uint32_t L2CacheTotalSize() const override;

	private:
		struct PlatformCPUInfo;

		PlatformCPUInfo* m_info = nullptr;
	};
}
