#include <interface.hpp>

#include <sdk.hpp>
#include <cstring>
#include <iostream>

Interface::Interface() :
	baseclass(nullptr),
	vtable(nullptr),
	vtableSize(0),
	isHooked(false),
	copy(nullptr)
{
}

Interface::Interface(void* baseclass, bool copyVtable, bool autoHook) : Interface() {
	this->baseclass = reinterpret_cast<uintptr_t**>(baseclass);
	this->vtable = *this->baseclass;
	while(this->vtable[this->vtableSize]) {
		++this->vtableSize;
	}
	if(copyVtable) {
		this->CopyVtable();
		if(autoHook) {
			this->EnableHooks();
		}
	}
}

Interface::~Interface() {
	this->DisableHooks();
	if(this->copy) {
		this->copy.reset();
	}
}

void Interface::CopyVtable() {
	if(!this->copy) {
		this->copy = std::make_unique<uintptr_t[]>(this->vtableSize + 1);
		std::memcpy(this->copy.get(), this->vtable - 1, sizeof(uintptr_t) + this->vtableSize * sizeof(uintptr_t));
	}
}

void Interface::EnableHooks() {
	if(!this->isHooked) {
		*this->baseclass = this->copy.get() + 1;
		this->isHooked = true;
	}
}

void Interface::DisableHooks() {
	if(this->isHooked) {
		*this->baseclass = this->vtable;
		this->isHooked = false;
	}
}

Interface* Interface::Create(void* ptr, bool copyVtable, bool autoHook) {
	return (ptr) ? new Interface(ptr, copyVtable, autoHook) : nullptr;
}

Interface* Interface::Create(const char* filename, const char* interfaceSymbol, bool copyVtable, bool autoHook) {
	auto ptr = Interface::GetPtr(filename, interfaceSymbol);
	return (ptr) ? new Interface(ptr, copyVtable, autoHook) : nullptr;
}

void Interface::Delete(Interface* ptr) {
	if(ptr) {
		delete ptr;
		ptr = nullptr;
	}
}

void* Interface::GetPtr(const char* filename, const char* interfaceSymbol) {
	auto handle = Memory::GetModuleHandleByName(filename);
	if(!handle) {
		std::cerr << "Plugin: Failed to open module " << filename << "!" << std::endl;
		return nullptr;
	}

	auto CreateInterface = Memory::GetSymbolAddress<CreateInterfaceFn>(handle, "CreateInterface");
	Memory::CloseModuleHandle(handle);

	if(!CreateInterface) {
		std::cerr << "Plugin: Failed to find symbol CreateInterface for " << filename << "!" << std::endl;
		return nullptr;
	}

	int ret;
	void* fn = CreateInterface(interfaceSymbol, &ret);

	if(ret) {
		std::cerr << "Failed to create interface with symbol " << interfaceSymbol << " in " << filename << "!" << std::endl;
		return nullptr;
	}

	return fn;
}
