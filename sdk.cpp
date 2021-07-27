#include <sdk.hpp>

#include <cstring>

// Code required to expose the plugin to source

InterfaceReg* InterfaceReg::s_pInterfaceRegs = nullptr;

static void* CreateInterfaceInternal(const char* pName, int* pReturnCode) {
	InterfaceReg* pCur;
	for(pCur = InterfaceReg::s_pInterfaceRegs; pCur; pCur = pCur->m_pNext) {
		if(!std::strcmp(pCur->m_pName, pName)) {
			if(pReturnCode) *pReturnCode = 0;
			return pCur->m_CreateFn();
		}
	}
	if(pReturnCode) *pReturnCode = 1;
	return nullptr;
}

extern "C" void* CreateInterface(const char* pName, int* pReturnCode) {
	return CreateInterfaceInternal(pName, pReturnCode);
}