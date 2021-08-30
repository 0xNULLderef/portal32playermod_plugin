#include <memory.hpp>

#include <link.h>
#include <string>
#include <cstring>
#include <unistd.h>

#define INRANGE(x, a, b) (x >= a && x <= b)
#define getBits(x) (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA) : (INRANGE(x, '0', '9') ? x - '0' : 0))
#define getByte(x) (getBits(x[0]) << 4 | getBits(x[1]))

std::vector<Memory::ModuleInfo> Memory::moduleList;

bool Memory::TryGetModule(const char* moduleName, Memory::ModuleInfo* info) {
	if(Memory::moduleList.empty()) {
		dl_iterate_phdr([](struct dl_phdr_info* info, size_t, void*) {
			auto module = Memory::ModuleInfo();
			auto temp = std::string(info->dlpi_name);
			auto index = temp.find_last_of("\\/");
			temp = temp.substr(index + 1, temp.length() - index);
			std::snprintf(module.name, sizeof(module.name), "%s", temp.c_str());
			module.base = info->dlpi_addr + info->dlpi_phdr[0].p_paddr;
			module.size = info->dlpi_phdr[0].p_memsz;
			std::strncpy(module.path, info->dlpi_name, sizeof(module.path));
			Memory::moduleList.push_back(module);
			return 0;
		}, nullptr);
	}
	for(Memory::ModuleInfo& item : Memory::moduleList) {
		if(!std::strcmp(item.name, moduleName)) {
			if(info) *info = item;
			return true;
		}
	}
	return false;
}
const char* Memory::GetModulePath(const char* moduleName) {
	auto info = Memory::ModuleInfo();
	return (Memory::TryGetModule(moduleName, &info)) ? std::string(info.path).c_str() : nullptr;
};
void* Memory::GetModuleHandleByName(const char* moduleName) {
	auto info = Memory::ModuleInfo();
	return (Memory::TryGetModule(moduleName, &info)) ? dlopen(info.path, RTLD_NOLOAD | RTLD_NOW) : nullptr;
}
void Memory::CloseModuleHandle(void* moduleHandle) {
	dlclose(moduleHandle);
}
std::string Memory::GetProcessName() {
	char link[32];
	char temp[MAX_PATH] = { 0 };
	std::sprintf(link, "/proc/%d/exe", getpid());
	readlink(link, temp, sizeof(temp));
	auto proc = std::string(temp);
	auto index = proc.find_last_of("\\/");
	proc = proc.substr(index + 1, proc.length() - index);
	return proc;
}
uintptr_t Memory::FindAddress(const uintptr_t start, const uintptr_t end, const char* target) {
	const char* pattern = target;
	uintptr_t result = 0;
	for(auto position = start; position < end; ++position) {
		if(!*pattern) return result;
		auto match = *reinterpret_cast<const uint8_t*>(pattern);
		auto byte = *reinterpret_cast<const uint8_t*>(position);
		if(match == '\?' || byte == getByte(pattern)) {
			if(!result) result = position;
			if(!pattern[2]) return result;
			pattern += (match != '\?') ? 3 : 2;
		} else {
			pattern = target;
			result = 0;
		}
	}
	return 0;
}
uintptr_t Memory::Scan(const char* moduleName, const char* pattern, int offset) {
	uintptr_t result = 0;
	auto info = Memory::ModuleInfo();
	if(Memory::TryGetModule(moduleName, &info)) {
		auto start = uintptr_t(info.base);
		auto end = start + info.size;
		result = Memory::FindAddress(start, end, pattern);
		if(result) result += offset;
	}
	return result;
}
std::vector<uintptr_t> Memory::MultiScan(const char* moduleName, const char* pattern, int offset) {
	std::vector<uintptr_t> result;
	auto length = std::strlen(pattern);
	auto info = Memory::ModuleInfo();
	if (Memory::TryGetModule(moduleName, &info)) {
		auto start = uintptr_t(info.base);
		auto end = start + info.size;
		auto addr = uintptr_t();
		while(true) {
			addr = Memory::FindAddress(start, end, pattern);
			if(addr) {
				result.push_back(addr + offset);
				start = addr + length;
			} else break;
		}
	}
	return result;
}
std::vector<uintptr_t> Memory::Scan(const char* moduleName, const Memory::Pattern* pattern) {
	std::vector<uintptr_t> result;
	auto info = Memory::ModuleInfo();
	if(Memory::TryGetModule(moduleName, &info)) {
		auto start = uintptr_t(info.base);
		auto end = start + info.size;
		auto addr = Memory::FindAddress(start, end, pattern->signature);
		if(addr) {
			for(auto const& offset : pattern->offsets) {
				result.push_back(addr + offset);
			}
		}
	}
	return result;
}
std::vector<std::vector<uintptr_t>> Memory::MultiScan(const char* moduleName, const Memory::Patterns* patterns) {
	auto results = std::vector<std::vector<uintptr_t>>();
	auto info = Memory::ModuleInfo();
	if (Memory::TryGetModule(moduleName, &info)) {
		auto moduleStart = uintptr_t(info.base);
		auto moduleEnd = moduleStart + info.size;
		for(const auto& pattern : *patterns) {
			auto length = std::strlen(pattern->signature);
			auto start = moduleStart;
			while(true) {
				auto addr = Memory::FindAddress(start, moduleEnd, pattern->signature);
				if(addr) {
					auto result = std::vector<uintptr_t>();
					for (const auto& offset : pattern->offsets) result.push_back(addr + offset);
					results.push_back(result);
					start = addr + length;
				} else break;
			}
		}
	}
	return results;
}
