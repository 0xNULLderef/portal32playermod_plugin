#pragma once

#include <cstring>
#include <cstdint>
#include <cmath>

#define __rescalll __attribute__((__cdecl__))
#define __rescall __attribute__((__cdecl__))

// --- Generic ---

struct Color {
	Color() { *((int*)this) = 255; }
	Color(int _r, int _g, int _b) { SetColor(_r, _g, _b, 255); }
	Color(int _r, int _g, int _b, int _a) { SetColor(_r, _g, _b, _a); }
	void SetColor(int _r, int _g, int _b, int _a = 255) {
		_color[0] =(unsigned char)_r;
		_color[1] =(unsigned char)_g;
		_color[2] =(unsigned char)_b;
		_color[3] =(unsigned char)_a;
	}
	unsigned char _color[4] = { 0, 0, 0, 0 };
};

// --- Source specific ---

typedef void*(*CreateInterfaceFn)(const char* pName, int* pReturnCode);
typedef void*(*InstantiateInterfaceFn)();

struct InterfaceReg {
	InstantiateInterfaceFn m_CreateFn;
	const char* m_pName;
	InterfaceReg* m_pNext;
	static InterfaceReg* s_pInterfaceRegs;

	InterfaceReg(InstantiateInterfaceFn fn, const char* pName)
		: m_pName(pName)
	{
		m_CreateFn = fn;
		m_pNext = s_pInterfaceRegs;
		s_pInterfaceRegs = this;
	}
};

// ISERVERPLUGINCALLBACKS002 is the second version of IServerPluginCallbacks
#define INTERFACEVERSION_ISERVERPLUGINCALLBACKS "ISERVERPLUGINCALLBACKS002"

// Macro to expose a globalvar
#define EXPOSE_SINGLE_INTERFACE_GLOBALVAR(className, interfaceName, versionName, globalVarName) \
	static void* __Create##className##interfaceName##_interface() { return static_cast<interfaceName*>(&globalVarName); } \
	static InterfaceReg __g_Create##className##interfaceName##_reg(__Create##className##interfaceName##_interface, versionName);

// Server plugin callbacks
class IServerPluginCallbacks {
public:
	virtual bool Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) = 0;
	virtual void Unload() = 0;
	virtual void Pause() = 0;
	virtual void UnPause() = 0;
	virtual const char* GetPluginDescription() = 0;
	virtual void LevelInit(char const* pMapName) = 0;
	virtual void ServerActivate(void* pEdictList, int edictCount, int clientMax) = 0;
	virtual void GameFrame(bool simulating) = 0;
	virtual void LevelShutdown() = 0;
	virtual void ClientFullyConnect(void* pEdict) = 0;
	virtual void ClientActive(void* pEntity) = 0;
	virtual void ClientDisconnect(void* pEntity) = 0;
	virtual void ClientPutInServer(void* pEntity, char const* playername) = 0;
	virtual void SetCommandClient(int index) = 0;
	virtual void ClientSettingsChanged(void* pEdict) = 0;
	virtual int ClientConnect(bool* bAllowConnect, void* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen) = 0;
	virtual int ClientCommand(void* pEntity, const void*& args) = 0;
	virtual int NetworkIDValidated(const char* pszUserName, const char* pszNetworkID) = 0;
	virtual void OnQueryCvarValueFinished(int iCookie, void* pPlayerEntity, int eStatus, const char* pCvarName, const char* pCvarValue) = 0;
	virtual void OnEdictAllocated(void* edict) = 0;
	virtual void OnEdictFreed(const void* edict) = 0;
};

// Logging system things - nescessary for some print functionality
typedef enum {
	LSEV_MESSAGE,
	LSEV_WARNING,
	LSEV_ASSERT,
	LSEV_ERROR,
} LoggingSeverity;

typedef enum {
	LCF_CONSOLE_ONLY = 0x1,
	LCF_DO_NOT_ECHO  = 0x2,
} LoggingChannelFlags;

struct LoggingContext {
	int channelID;
	LoggingChannelFlags flags;
	LoggingSeverity severity;
	Color color;
};

class ILoggingListener {
public:
	virtual void Log(const LoggingContext *ctx, const char *msg) = 0;
};

// -- VScript stuff -- HIGHLY EXPERIMENTAL

enum ScriptLanguage_t {
	SL_NONE,
	SL_GAMEMONKEY,
	SL_SQUIRREL,
	SL_LUA,
	SL_PYTHON,
	SL_DEFAULT = SL_SQUIRREL
};

// typedef void* IScriptVM;

#define Assert(_exp) ((void)0)

enum ScriptStatus_t {
	SCRIPT_ERROR = -1,
	SCRIPT_DONE,
	SCRIPT_RUNNING,
};

#define DECLARE_POINTER_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__* name
DECLARE_POINTER_HANDLE(HSCRIPT);
#define INVALID_HSCRIPT ((HSCRIPT)-1)

typedef enum _fieldtypes {
	FIELD_VOID = 0,
	FIELD_FLOAT,
	FIELD_STRING,
	FIELD_VECTOR,
	FIELD_QUATERNION,
	FIELD_INTEGER,
	FIELD_BOOLEAN,
	FIELD_SHORT,
	FIELD_CHARACTER,
	FIELD_COLOR32,
	FIELD_EMBEDDED,
	FIELD_CUSTOM,
	FIELD_CLASSPTR,
	FIELD_EHANDLE,
	FIELD_EDICT,

	FIELD_POSITION_VECTOR,
	FIELD_TIME,
	FIELD_TICK,
	FIELD_MODELNAME,
	FIELD_SOUNDNAME,
	FIELD_INPUT,
	FIELD_FUNCTION,
	FIELD_VMATRIX,
	FIELD_VMATRIX_WORLDSPACE,
	FIELD_MATRIX3X4_WORLDSPACE,
	FIELD_INTERVAL,
	FIELD_MODELINDEX,
	FIELD_MATERIALINDEX,
	FIELD_VECTOR2D,
	FIELD_INTEGER64,
	FIELD_VECTOR4D,
	FIELD_TYPECOUNT,
} fieldtype_t;

enum ExtendedFieldType {
	FIELD_TYPEUNKNOWN = FIELD_TYPECOUNT,
	FIELD_CSTRING,
	FIELD_HSCRIPT,
	FIELD_VARIANT,
};

enum SVFlags_t {
	SV_FREE = 0x01,
};

typedef short int16;
typedef long long int64;

struct Vector {
	float x, y, z;
	inline Vector() : x(0), y(y), z(0) {}
	inline Vector(float x, float y, float z) : x(x), y(y), z(z) {}
	inline void Init(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

struct ScriptVariant_t;

template <typename T> struct ScriptDeducer { /*enum { FIELD_TYPE = FIELD_TYPEUNKNOWN };*/ };
#define DECLARE_DEDUCE_FIELDTYPE( fieldType, type ) template<> struct ScriptDeducer<type> { enum { FIELD_TYPE = fieldType }; };

DECLARE_DEDUCE_FIELDTYPE( FIELD_VOID,		void );
DECLARE_DEDUCE_FIELDTYPE( FIELD_FLOAT,		float );
DECLARE_DEDUCE_FIELDTYPE( FIELD_CSTRING,	const char * );
DECLARE_DEDUCE_FIELDTYPE( FIELD_CSTRING,	char * );
DECLARE_DEDUCE_FIELDTYPE( FIELD_VECTOR,		Vector );
DECLARE_DEDUCE_FIELDTYPE( FIELD_VECTOR,		const Vector &);
DECLARE_DEDUCE_FIELDTYPE( FIELD_INTEGER,	int );
DECLARE_DEDUCE_FIELDTYPE( FIELD_BOOLEAN,	bool );
DECLARE_DEDUCE_FIELDTYPE( FIELD_CHARACTER,	char );
DECLARE_DEDUCE_FIELDTYPE( FIELD_HSCRIPT,	HSCRIPT );
DECLARE_DEDUCE_FIELDTYPE( FIELD_VARIANT,	ScriptVariant_t );

#define ScriptDeduceType( T ) ScriptDeducer<T>::FIELD_TYPE

typedef int ScriptDataType_t;

struct ScriptVariant_t {
	ScriptVariant_t() :						m_flags(0), m_type(FIELD_VOID)		{ m_pVector = 0; }
	ScriptVariant_t(int val) :				m_flags(0), m_type(FIELD_INTEGER)	{ m_int = val;}
	ScriptVariant_t(float val) :			m_flags(0), m_type(FIELD_FLOAT)		{ m_float = val; }
	ScriptVariant_t(double val) :			m_flags(0), m_type(FIELD_FLOAT)		{ m_float = (float)val; }
	ScriptVariant_t(char val) :				m_flags(0), m_type(FIELD_CHARACTER)	{ m_char = val; }
	ScriptVariant_t(bool val) :				m_flags(0), m_type(FIELD_BOOLEAN)	{ m_bool = val; }
	ScriptVariant_t(HSCRIPT val) :			m_flags(0), m_type(FIELD_HSCRIPT)	{ m_hScript = val; }

	ScriptVariant_t(const Vector &val, bool bCopy = false) :	m_flags(0), m_type(FIELD_VECTOR)	{ if(!bCopy) { m_pVector = &val; } else { m_pVector = new Vector(val); m_flags |= SV_FREE; } }
	ScriptVariant_t(const Vector* val, bool bCopy = false) :	m_flags(0), m_type(FIELD_VECTOR)	{ if(!bCopy) { m_pVector = val; } else { m_pVector = new Vector(*val); m_flags |= SV_FREE; } }
	ScriptVariant_t(const char* val , bool bCopy = false) :		m_flags(0), m_type(FIELD_CSTRING)	{ if(!bCopy) { m_pszString = val; } else { m_pszString = strdup(val); m_flags |= SV_FREE; } }

	bool IsNull() const						{ return (m_type == FIELD_VOID); }

	operator int() const					{ Assert(m_type == FIELD_INTEGER);		return m_int; }
	operator int64() const					{ Assert(m_type == FIELD_INTEGER);		return static_cast<int64>(m_int); }
	operator float() const					{ Assert(m_type == FIELD_FLOAT);		return m_float; }
	operator const char* () const			{ Assert(m_type == FIELD_CSTRING);		return (m_pszString) ? m_pszString : ""; }
	operator const Vector &() const			{ Assert(m_type == FIELD_VECTOR);		static Vector vecNull(0, 0, 0); return (m_pVector) ? *m_pVector : vecNull; }
	operator char() const					{ Assert(m_type == FIELD_CHARACTER);	return m_char; }
	operator bool() const					{ Assert(m_type == FIELD_BOOLEAN);		return m_bool; }
	operator HSCRIPT() const				{ Assert(m_type == FIELD_HSCRIPT);		return m_hScript; }

	void operator=(int i) 					{ m_type = FIELD_INTEGER; 	m_int = i; }
	void operator=(int64 i) 				{ m_type = FIELD_INTEGER; 	m_int = i; }
	void operator=(float f) 				{ m_type = FIELD_FLOAT; 	m_float = f; }
	void operator=(double f) 				{ m_type = FIELD_FLOAT; 	m_float = (float)f; }
	void operator=(const Vector &vec)		{ m_type = FIELD_VECTOR; 	m_pVector = &vec; }
	void operator=(const Vector* vec)		{ m_type = FIELD_VECTOR; 	m_pVector = vec; }
	void operator=(const char* psz)			{ m_type = FIELD_CSTRING; 	m_pszString = psz; }
	void operator=(char c)					{ m_type = FIELD_CHARACTER; m_char = c; }
	void operator=(bool b) 					{ m_type = FIELD_BOOLEAN; 	m_bool = b; }
	void operator=(HSCRIPT h) 				{ m_type = FIELD_HSCRIPT; 	m_hScript = h; }

	void Free()								{ if((m_flags & SV_FREE) && (m_type == FIELD_HSCRIPT || m_type == FIELD_VECTOR || m_type == FIELD_CSTRING)) delete m_pszString; } // Generally only needed for return results

	template <typename T>
	T Get() {
		T value;
		AssignTo(&value);
		return value;
	}

	template <typename T>
	bool AssignTo(T* pDest) {
		ScriptDataType_t destType = ScriptDeduceType(T);
		if(destType == m_type) {
			*pDest = *this;
			return true;
		}

		if(m_type != FIELD_VECTOR && m_type != FIELD_CSTRING && destType != FIELD_VECTOR && destType != FIELD_CSTRING) {
			switch(m_type) {
			case FIELD_VOID:		*pDest = 0; 		break;
			case FIELD_INTEGER:		*pDest = m_int; 	return true;
			case FIELD_FLOAT:		*pDest = m_float; 	return true;
			case FIELD_CHARACTER:	*pDest = m_char; 	return true;
			case FIELD_BOOLEAN:		*pDest = m_bool; 	return true;
			case FIELD_HSCRIPT:		*pDest = m_hScript; return true;
			}
		} else if(destType != FIELD_VECTOR) {
			*pDest = 0;
		}
		return false;
	}

	bool AssignTo(float* pDest) {
		switch(m_type) {
		case FIELD_VOID:		*pDest = 0; 		return false;
		case FIELD_INTEGER:		*pDest = m_int; 	return true;
		case FIELD_FLOAT:		*pDest = m_float; 	return true;
		case FIELD_BOOLEAN:		*pDest = m_bool; 	return true;
		default:
			return false;
		}
	}

	bool AssignTo(int* pDest) {
		switch(m_type) {
		case FIELD_VOID:		*pDest = 0; 			return false;
		case FIELD_INTEGER:		*pDest = m_int; 		return true;
		case FIELD_FLOAT:		*pDest = (int)m_float; 	return true;
		case FIELD_BOOLEAN:		*pDest = m_bool; 		return true;
		default:
			return false;
		}
	}

	bool AssignTo(bool* pDest) {
		switch(m_type) {
		case FIELD_VOID:		*pDest = 0; 		return false;
		case FIELD_INTEGER:		*pDest = m_int; 	return true;
		case FIELD_FLOAT:		*pDest = m_float; 	return true;
		case FIELD_BOOLEAN:		*pDest = m_bool; 	return true;
		default:
			return false;
		}
	}

	bool AssignTo(char **pDest) {
		*pDest = "";
		return false;
	}

	bool AssignTo(ScriptVariant_t* pDest) {
		pDest->m_type = m_type;
		if(m_type == FIELD_VECTOR) {
			pDest->m_pVector = new Vector;
			((Vector *)(pDest->m_pVector))->Init(m_pVector->x, m_pVector->y, m_pVector->z);
			pDest->m_flags |= SV_FREE;
		}
		else if(m_type == FIELD_CSTRING) {
			pDest->m_pszString = strdup(m_pszString);
			pDest->m_flags |= SV_FREE;
		} else {
			pDest->m_int = m_int;
		}
		return false;
	}

	union {
		int				m_int;
		float			m_float;
		const char*		m_pszString;
		const Vector*	m_pVector;
		char			m_char;
		bool			m_bool;
		HSCRIPT			m_hScript;
	};

	int16				m_type;
	int16				m_flags;
};

template <class T, class I = int> struct CUtlMemory {
	T* m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;
};

template <class T, class A = CUtlMemory<T>> struct CUtlVector {
	A m_Memory;
	int m_Size;
	T* m_pElements;

	void Append(const T& val) {
		if(this->m_Size == this->m_Memory.m_nAllocationCount) {
			int grow = this->m_Memory.m_nGrowSize;
			if(grow == 0)
				grow = 1;
			this->m_Memory.m_nAllocationCount += grow;
			this->m_Memory.m_pMemory = static_cast<T*>(realloc(this->m_Memory.m_pMemory, sizeof(T) * this->m_Memory.m_nAllocationCount));
			this->m_pElements = this->m_Memory.m_pMemory;
		}
		this->m_Memory.m_pMemory[this->m_Size] = val;
		this->m_Size++;
	}

	void Clear() {
		if(this->m_Memory.m_pMemory) {
			free(this->m_Memory.m_pMemory);
			this->m_Memory.m_pMemory = 0;
		}
		this->m_Size = 0;
		this->m_Memory.m_nAllocationCount = 0;
	}
};

struct ScriptFuncDescriptor_t {
	ScriptFuncDescriptor_t() {
		m_pszFunction = NULL;
		m_ReturnType = FIELD_TYPEUNKNOWN;
		m_pszDescription = NULL;
	}

	const char* m_pszScriptName;
	const char* m_pszFunction;
	const char* m_pszDescription;
	ScriptDataType_t m_ReturnType;
	CUtlVector<ScriptDataType_t> m_Parameters;
};

typedef void* ScriptFunctionBindingStorageType_t;
typedef bool (*ScriptBindingFunc_t)(ScriptFunctionBindingStorageType_t pFunction, void* pContext, ScriptVariant_t* pArguments, int nArguments, ScriptVariant_t* pReturn);

struct ScriptFunctionBinding_t {
	ScriptFuncDescriptor_t				m_desc;
	ScriptBindingFunc_t					m_pfnBinding;
	ScriptFunctionBindingStorageType_t	m_pFunction;
	unsigned							m_flags;
};

class IScriptInstanceHelper {
public:
	virtual void* GetProxied(void* p)											{ return p; }
	virtual bool ToString(void* p, char* pBuf, int bufSize)						{ return false; }
	virtual void* BindOnRead(HSCRIPT hInstance, void* pOld, const char* pszId)	{ return NULL; }
};

struct ScriptClassDesc_t {
	ScriptClassDesc_t(void (*pfnInitializer)()) : m_pszScriptName(0), m_pszClassname(0), m_pszDescription(0), m_pBaseDesc(0), m_pfnConstruct(0), m_pfnDestruct(0), pHelper(NULL)  {
		(*pfnInitializer)();
		ScriptClassDesc_t **ppHead = GetDescList();
		m_pNextDesc = *ppHead;
		*ppHead = this;
	}

	const char*							m_pszScriptName;
	const char*							m_pszClassname;
	const char*							m_pszDescription;
	ScriptClassDesc_t*					m_pBaseDesc;
	CUtlVector<ScriptFunctionBinding_t> m_FunctionBindings;

	void* (*m_pfnConstruct)();
	void (*m_pfnDestruct)(void *);
	IScriptInstanceHelper*				pHelper; // optional helper

	ScriptClassDesc_t*					m_pNextDesc;

	static ScriptClassDesc_t** GetDescList() {
		static ScriptClassDesc_t* pHead;
		return &pHead;
	}
};

enum ScriptErrorLevel_t {
	SCRIPT_LEVEL_WARNING	= 0,
	SCRIPT_LEVEL_ERROR,
};

class CUtlBuffer;
typedef void (*ScriptOutputFunc_t)(const char* pszText);
typedef bool (*ScriptErrorFunc_t)(ScriptErrorLevel_t eLevel, const char* pszText);

#define ARRAYSIZE(p) (sizeof(p)/sizeof(p[0]))

class IScriptVM {
public:
	virtual bool Init() = 0;
	virtual void Shutdown() = 0;

	virtual bool ConnectDebugger() = 0;
	virtual void DisconnectDebugger() = 0;

	virtual ScriptLanguage_t GetLanguage() = 0;
	virtual const char* GetLanguageName() = 0;

	virtual void AddSearchPath(const char* pszSearchPath) = 0;
 
 	virtual bool Frame(float simTime) = 0;

	virtual ScriptStatus_t Run(const char* pszScript, bool bWait = true) = 0;
	inline ScriptStatus_t Run(const unsigned char* pszScript, bool bWait = true) { return Run((char *)pszScript, bWait); }

 	virtual HSCRIPT CompileScript(const char* pszScript, const char* pszId = NULL) = 0;
	inline HSCRIPT CompileScript(const unsigned char* pszScript, const char* pszId = NULL) { return CompileScript((char *)pszScript, pszId); }
	virtual void ReleaseScript(HSCRIPT) = 0;

	virtual ScriptStatus_t Run(HSCRIPT hScript, HSCRIPT hScope = NULL, bool bWait = true) = 0;
	virtual ScriptStatus_t Run(HSCRIPT hScript, bool bWait) = 0;

	virtual HSCRIPT CreateScope(const char* pszScope, HSCRIPT hParent = NULL) = 0;
	virtual void ReleaseScope(HSCRIPT hScript) = 0;

	virtual HSCRIPT LookupFunction(const char* pszFunction, HSCRIPT hScope = NULL) = 0;
	virtual void ReleaseFunction(HSCRIPT hScript) = 0;

	virtual ScriptStatus_t ExecuteFunction(HSCRIPT hFunction, ScriptVariant_t* pArgs, int nArgs, ScriptVariant_t* pReturn, HSCRIPT hScope, bool bWait) = 0;

	virtual void RegisterFunction(ScriptFunctionBinding_t* pScriptFunction) = 0;

	virtual bool RegisterClass(ScriptClassDesc_t* pClassDesc) = 0;

	void RegisterAllClasses() {
		ScriptClassDesc_t* pCurrent = *ScriptClassDesc_t::GetDescList();
		while(pCurrent) {
			RegisterClass(pCurrent);
			pCurrent = pCurrent->m_pNextDesc;
		}
	}

	virtual HSCRIPT RegisterInstance(ScriptClassDesc_t* pDesc, void* pInstance) = 0;
	virtual void SetInstanceUniqeId(HSCRIPT hInstance, const char* pszId) = 0;
	template <typename T> HSCRIPT RegisterInstance(T* pInstance)																	{ return RegisterInstance(GetScriptDesc(pInstance), pInstance);	}
	template <typename T> HSCRIPT RegisterInstance(T* pInstance, const char* pszInstance, HSCRIPT hScope = NULL)					{ HSCRIPT hInstance = RegisterInstance(GetScriptDesc(pInstance), pInstance); SetValue(hScope, pszInstance, hInstance); return hInstance; }
	virtual void RemoveInstance(HSCRIPT) = 0;
	void RemoveInstance(HSCRIPT hInstance, const char* pszInstance, HSCRIPT hScope = NULL)											{ ClearValue(hScope, pszInstance); RemoveInstance(hInstance); }
	void RemoveInstance(const char* pszInstance, HSCRIPT hScope = NULL)																{ ScriptVariant_t val; if(GetValue(hScope, pszInstance, &val)) { if(val.m_type == FIELD_HSCRIPT) { RemoveInstance(val, pszInstance, hScope); } ReleaseValue(val); } }

	virtual void* GetInstanceValue(HSCRIPT hInstance, ScriptClassDesc_t* pExpectedType = NULL) = 0;

	virtual bool GenerateUniqueKey(const char* pszRoot, char* pBuf, int nBufSize) = 0;

	virtual bool ValueExists(HSCRIPT hScope, const char* pszKey) = 0;
	bool ValueExists(const char* pszKey)																							{ return ValueExists(NULL, pszKey); }

	virtual bool SetValue(HSCRIPT hScope, const char* pszKey, const char* pszValue) = 0;
	virtual bool SetValue(HSCRIPT hScope, const char* pszKey, const ScriptVariant_t &value) = 0;
	bool SetValue(const char* pszKey, const ScriptVariant_t &value)																	{ return SetValue(NULL, pszKey, value); }

	virtual void CreateTable(ScriptVariant_t &Table) = 0;
	virtual int	GetNumTableEntries(HSCRIPT hScope) = 0;
	virtual int GetKeyValue(HSCRIPT hScope, int nIterator, ScriptVariant_t* pKey, ScriptVariant_t* pValue) = 0;

	virtual bool GetValue(HSCRIPT hScope, const char* pszKey, ScriptVariant_t* pValue) = 0;
	bool GetValue(const char* pszKey, ScriptVariant_t* pValue)																		{ return GetValue(NULL, pszKey, pValue); }
	virtual void ReleaseValue(ScriptVariant_t &value) = 0;

	virtual bool ClearValue(HSCRIPT hScope, const char* pszKey) = 0;
	bool ClearValue(const char* pszKey)																								{ return ClearValue(NULL, pszKey); }

	virtual void WriteState(CUtlBuffer* pBuffer) = 0;
	virtual void ReadState(CUtlBuffer* pBuffer) = 0;
	virtual void RemoveOrphanInstances() = 0;

	virtual void DumpState() = 0;

	virtual void SetOutputCallback(ScriptOutputFunc_t pFunc) = 0;
	virtual void SetErrorCallback(ScriptErrorFunc_t pFunc) = 0;

	virtual bool RaiseException(const char* pszExceptionText) = 0;

	ScriptStatus_t Call(HSCRIPT hFunction, HSCRIPT hScope = NULL, bool bWait = true, ScriptVariant_t* pReturn = NULL) {
		return ExecuteFunction(hFunction, NULL, 0, pReturn, hScope, bWait);
	}

	template <typename ARG_TYPE_1>
	ScriptStatus_t Call(HSCRIPT hFunction, HSCRIPT hScope, bool bWait, ScriptVariant_t* pReturn, ARG_TYPE_1 arg1) {
		ScriptVariant_t args[1]; args[0] = arg1;
		return ExecuteFunction(hFunction, args, ARRAYSIZE(args), pReturn, hScope, bWait);
	}

	template <typename ARG_TYPE_1, typename ARG_TYPE_2>
	ScriptStatus_t Call(HSCRIPT hFunction, HSCRIPT hScope, bool bWait, ScriptVariant_t* pReturn, ARG_TYPE_1 arg1, ARG_TYPE_2 arg2) {
		ScriptVariant_t args[2]; args[0] = arg1; args[1] = arg2;
		return ExecuteFunction(hFunction, args, ARRAYSIZE(args), pReturn, hScope, bWait);
	}

	template <typename ARG_TYPE_1, typename ARG_TYPE_2, typename	ARG_TYPE_3>
	ScriptStatus_t Call(HSCRIPT hFunction, HSCRIPT hScope, bool bWait, ScriptVariant_t* pReturn, ARG_TYPE_1 arg1, ARG_TYPE_2 arg2, ARG_TYPE_3 arg3) {
		ScriptVariant_t args[3]; args[0] = arg1; args[1] = arg2; args[2] = arg3;
		return ExecuteFunction(hFunction, args, ARRAYSIZE(args), pReturn, hScope, bWait);
	}

	template <typename ARG_TYPE_1, typename ARG_TYPE_2, typename	ARG_TYPE_3,	typename ARG_TYPE_4>
	ScriptStatus_t Call(HSCRIPT hFunction, HSCRIPT hScope, bool bWait, ScriptVariant_t* pReturn, ARG_TYPE_1 arg1, ARG_TYPE_2 arg2, ARG_TYPE_3 arg3, ARG_TYPE_4 arg4) {
		ScriptVariant_t args[4]; args[0] = arg1; args[1] = arg2; args[2] = arg3; args[3] = arg4;
		return ExecuteFunction(hFunction, args, ARRAYSIZE(args), pReturn, hScope, bWait);
	}

	template <typename ARG_TYPE_1, typename ARG_TYPE_2, typename	ARG_TYPE_3,	typename ARG_TYPE_4, typename ARG_TYPE_5>
	ScriptStatus_t Call(HSCRIPT hFunction, HSCRIPT hScope, bool bWait, ScriptVariant_t* pReturn, ARG_TYPE_1 arg1, ARG_TYPE_2 arg2, ARG_TYPE_3 arg3, ARG_TYPE_4 arg4, ARG_TYPE_5 arg5) {
		ScriptVariant_t args[5]; args[0] = arg1; args[1] = arg2; args[2] = arg3; args[3] = arg4; args[4] = arg5;
		return ExecuteFunction(hFunction, args, ARRAYSIZE(args), pReturn, hScope, bWait);
	}

	template <typename ARG_TYPE_1, typename ARG_TYPE_2, typename	ARG_TYPE_3,	typename ARG_TYPE_4, typename ARG_TYPE_5, typename ARG_TYPE_6>
	ScriptStatus_t Call(HSCRIPT hFunction, HSCRIPT hScope, bool bWait, ScriptVariant_t* pReturn, ARG_TYPE_1 arg1, ARG_TYPE_2 arg2, ARG_TYPE_3 arg3, ARG_TYPE_4 arg4, ARG_TYPE_5 arg5, ARG_TYPE_6 arg6) {
		ScriptVariant_t args[6]; args[0] = arg1; args[1] = arg2; args[2] = arg3; args[3] = arg4; args[4] = arg5; args[5] = arg6;
		return ExecuteFunction(hFunction, args, ARRAYSIZE(args), pReturn, hScope, bWait);
	}

	template <typename ARG_TYPE_1, typename ARG_TYPE_2, typename	ARG_TYPE_3,	typename ARG_TYPE_4, typename ARG_TYPE_5, typename ARG_TYPE_6, typename ARG_TYPE_7>
	ScriptStatus_t Call(HSCRIPT hFunction, HSCRIPT hScope, bool bWait, ScriptVariant_t* pReturn, ARG_TYPE_1 arg1, ARG_TYPE_2 arg2, ARG_TYPE_3 arg3, ARG_TYPE_4 arg4, ARG_TYPE_5 arg5, ARG_TYPE_6 arg6, ARG_TYPE_7 arg7) {
		ScriptVariant_t args[7]; args[0] = arg1; args[1] = arg2; args[2] = arg3; args[3] = arg4; args[4] = arg5; args[5] = arg6; args[6] = arg7;
		return ExecuteFunction(hFunction, args, ARRAYSIZE(args), pReturn, hScope, bWait);
	}

	template <typename ARG_TYPE_1, typename ARG_TYPE_2, typename	ARG_TYPE_3,	typename ARG_TYPE_4, typename ARG_TYPE_5, typename ARG_TYPE_6, typename ARG_TYPE_7, typename ARG_TYPE_8>
	ScriptStatus_t Call(HSCRIPT hFunction, HSCRIPT hScope, bool bWait, ScriptVariant_t* pReturn, ARG_TYPE_1 arg1, ARG_TYPE_2 arg2, ARG_TYPE_3 arg3, ARG_TYPE_4 arg4, ARG_TYPE_5 arg5, ARG_TYPE_6 arg6, ARG_TYPE_7 arg7, ARG_TYPE_8 arg8) {
		ScriptVariant_t args[8]; args[0] = arg1; args[1] = arg2; args[2] = arg3; args[3] = arg4; args[4] = arg5; args[5] = arg6; args[6] = arg7; args[7] = arg8;
		return ExecuteFunction(hFunction, args, ARRAYSIZE(args), pReturn, hScope, bWait);
	}

	template <typename ARG_TYPE_1, typename ARG_TYPE_2, typename	ARG_TYPE_3,	typename ARG_TYPE_4, typename ARG_TYPE_5, typename ARG_TYPE_6, typename ARG_TYPE_7, typename ARG_TYPE_8, typename ARG_TYPE_9>
	ScriptStatus_t Call(HSCRIPT hFunction, HSCRIPT hScope, bool bWait, ScriptVariant_t* pReturn, ARG_TYPE_1 arg1, ARG_TYPE_2 arg2, ARG_TYPE_3 arg3, ARG_TYPE_4 arg4, ARG_TYPE_5 arg5, ARG_TYPE_6 arg6, ARG_TYPE_7 arg7, ARG_TYPE_8 arg8, ARG_TYPE_9 arg9) {
		ScriptVariant_t args[9]; args[0] = arg1; args[1] = arg2; args[2] = arg3; args[3] = arg4; args[4] = arg5; args[5] = arg6; args[6] = arg7; args[7] = arg8; args[8] = arg9;
		return ExecuteFunction(hFunction, args, ARRAYSIZE(args), pReturn, hScope, bWait);
	}

	template <typename ARG_TYPE_1, typename ARG_TYPE_2, typename	ARG_TYPE_3,	typename ARG_TYPE_4, typename ARG_TYPE_5, typename ARG_TYPE_6, typename ARG_TYPE_7, typename ARG_TYPE_8, typename ARG_TYPE_9, typename ARG_TYPE_10>
	ScriptStatus_t Call(HSCRIPT hFunction, HSCRIPT hScope, bool bWait, ScriptVariant_t* pReturn, ARG_TYPE_1 arg1, ARG_TYPE_2 arg2, ARG_TYPE_3 arg3, ARG_TYPE_4 arg4, ARG_TYPE_5 arg5, ARG_TYPE_6 arg6, ARG_TYPE_7 arg7, ARG_TYPE_8 arg8, ARG_TYPE_9 arg9, ARG_TYPE_10 arg10) {
		ScriptVariant_t args[10]; args[0] = arg1; args[1] = arg2; args[2] = arg3; args[3] = arg4; args[4] = arg5; args[5] = arg6; args[6] = arg7; args[7] = arg8; args[8] = arg9; args[9] = arg10;
		return ExecuteFunction(hFunction, args, ARRAYSIZE(args), pReturn, hScope, bWait);
	}

	template <typename ARG_TYPE_1, typename ARG_TYPE_2, typename	ARG_TYPE_3,	typename ARG_TYPE_4, typename ARG_TYPE_5, typename ARG_TYPE_6, typename ARG_TYPE_7, typename ARG_TYPE_8, typename ARG_TYPE_9, typename ARG_TYPE_10, typename ARG_TYPE_11>
	ScriptStatus_t Call(HSCRIPT hFunction, HSCRIPT hScope, bool bWait, ScriptVariant_t* pReturn, ARG_TYPE_1 arg1, ARG_TYPE_2 arg2, ARG_TYPE_3 arg3, ARG_TYPE_4 arg4, ARG_TYPE_5 arg5, ARG_TYPE_6 arg6, ARG_TYPE_7 arg7, ARG_TYPE_8 arg8, ARG_TYPE_9 arg9, ARG_TYPE_10 arg10, ARG_TYPE_11 arg11) {
		ScriptVariant_t args[11]; args[0] = arg1; args[1] = arg2; args[2] = arg3; args[3] = arg4; args[4] = arg5; args[5] = arg6; args[6] = arg7; args[7] = arg8; args[8] = arg9; args[9] = arg10; args[10] = arg11;
		return ExecuteFunction(hFunction, args, ARRAYSIZE(args), pReturn, hScope, bWait);
	}

	template <typename ARG_TYPE_1, typename ARG_TYPE_2, typename	ARG_TYPE_3,	typename ARG_TYPE_4, typename ARG_TYPE_5, typename ARG_TYPE_6, typename ARG_TYPE_7, typename ARG_TYPE_8, typename ARG_TYPE_9, typename ARG_TYPE_10, typename ARG_TYPE_11, typename ARG_TYPE_12>
	ScriptStatus_t Call(HSCRIPT hFunction, HSCRIPT hScope, bool bWait, ScriptVariant_t* pReturn, ARG_TYPE_1 arg1, ARG_TYPE_2 arg2, ARG_TYPE_3 arg3, ARG_TYPE_4 arg4, ARG_TYPE_5 arg5, ARG_TYPE_6 arg6, ARG_TYPE_7 arg7, ARG_TYPE_8 arg8, ARG_TYPE_9 arg9, ARG_TYPE_10 arg10, ARG_TYPE_11 arg11, ARG_TYPE_12 arg12) {
		ScriptVariant_t args[12]; args[0] = arg1; args[1] = arg2; args[2] = arg3; args[3] = arg4; args[4] = arg5; args[5] = arg6; args[6] = arg7; args[7] = arg8; args[8] = arg9; args[9] = arg10; args[10] = arg11; args[11] = arg12;
		return ExecuteFunction(hFunction, args, ARRAYSIZE(args), pReturn, hScope, bWait);
	}

	template <typename ARG_TYPE_1, typename ARG_TYPE_2, typename	ARG_TYPE_3,	typename ARG_TYPE_4, typename ARG_TYPE_5, typename ARG_TYPE_6, typename ARG_TYPE_7, typename ARG_TYPE_8, typename ARG_TYPE_9, typename ARG_TYPE_10, typename ARG_TYPE_11, typename ARG_TYPE_12, typename ARG_TYPE_13>
	ScriptStatus_t Call(HSCRIPT hFunction, HSCRIPT hScope, bool bWait, ScriptVariant_t* pReturn, ARG_TYPE_1 arg1, ARG_TYPE_2 arg2, ARG_TYPE_3 arg3, ARG_TYPE_4 arg4, ARG_TYPE_5 arg5, ARG_TYPE_6 arg6, ARG_TYPE_7 arg7, ARG_TYPE_8 arg8, ARG_TYPE_9 arg9, ARG_TYPE_10 arg10, ARG_TYPE_11 arg11, ARG_TYPE_12 arg12, ARG_TYPE_13 arg13) {
		ScriptVariant_t args[13]; args[0] = arg1; args[1] = arg2; args[2] = arg3; args[3] = arg4; args[4] = arg5; args[5] = arg6; args[6] = arg7; args[7] = arg8; args[8] = arg9; args[9] = arg10; args[10] = arg11; args[11] = arg12; args[12] = arg13;
		return ExecuteFunction(hFunction, args, ARRAYSIZE(args), pReturn, hScope, bWait);
	}

	template <typename ARG_TYPE_1, typename ARG_TYPE_2, typename	ARG_TYPE_3,	typename ARG_TYPE_4, typename ARG_TYPE_5, typename ARG_TYPE_6, typename ARG_TYPE_7, typename ARG_TYPE_8, typename ARG_TYPE_9, typename ARG_TYPE_10, typename ARG_TYPE_11, typename ARG_TYPE_12, typename ARG_TYPE_13, typename ARG_TYPE_14>
	ScriptStatus_t Call(HSCRIPT hFunction, HSCRIPT hScope, bool bWait, ScriptVariant_t* pReturn, ARG_TYPE_1 arg1, ARG_TYPE_2 arg2, ARG_TYPE_3 arg3, ARG_TYPE_4 arg4, ARG_TYPE_5 arg5, ARG_TYPE_6 arg6, ARG_TYPE_7 arg7, ARG_TYPE_8 arg8, ARG_TYPE_9 arg9, ARG_TYPE_10 arg10, ARG_TYPE_11 arg11, ARG_TYPE_12 arg12, ARG_TYPE_13 arg13, ARG_TYPE_14 arg14) {
		ScriptVariant_t args[14]; args[0] = arg1; args[1] = arg2; args[2] = arg3; args[3] = arg4; args[4] = arg5; args[5] = arg6; args[6] = arg7; args[7] = arg8; args[8] = arg9; args[9] = arg10; args[10] = arg11; args[11] = arg12; args[12] = arg13; args[13] = arg14; 
		return ExecuteFunction(hFunction, args, ARRAYSIZE(args), pReturn, hScope, bWait);
	}
};

// Only support funcs with 0 arguments for now, fuck that macro

template <typename FUNCPTR_TYPE>
inline FUNCPTR_TYPE ScriptConvertFreeFuncPtrFromVoid(ScriptFunctionBindingStorageType_t p) {
	if(sizeof(FUNCPTR_TYPE) == 4 ) {
		union FuncPtrConvertMI {
			FUNCPTR_TYPE pFunc;
			ScriptFunctionBindingStorageType_t stype;
		};

		FuncPtrConvertMI convert;
		convert.pFunc = 0;
		convert.stype = p;
		return convert.pFunc;
	} else {
		union FuncPtrConvertMI {
			FUNCPTR_TYPE pFunc;
			struct {
				ScriptFunctionBindingStorageType_t stype;
				intptr_t iToc;
			} fn8;
		};

		FuncPtrConvertMI convert;
		convert.pFunc = 0;
		convert.fn8.stype = p;
		convert.fn8.iToc = 0;
		return convert.pFunc;
	}
}

template <typename FUNCPTR_TYPE>
inline FUNCPTR_TYPE ScriptConvertFuncPtrFromVoid( ScriptFunctionBindingStorageType_t p ) {
	return ScriptConvertFreeFuncPtrFromVoid<FUNCPTR_TYPE>(p);
}

template <typename FUNCPTR_TYPE>
inline ScriptFunctionBindingStorageType_t ScriptConvertFreeFuncPtrToVoid(FUNCPTR_TYPE pFunc) {
	if(sizeof( FUNCPTR_TYPE) == 4) {
		union FuncPtrConvertMI {
			FUNCPTR_TYPE pFunc;
			ScriptFunctionBindingStorageType_t stype;
		};

		FuncPtrConvertMI convert;
		convert.pFunc = pFunc;
		return convert.stype;
	} else {
		union FuncPtrConvertMI {
			FUNCPTR_TYPE pFunc;
			struct {
				ScriptFunctionBindingStorageType_t stype;
				intptr_t iToc;
			} fn8;
		};

		FuncPtrConvertMI convert;
		convert.fn8.iToc = 0;
		convert.pFunc = pFunc;
		if (!convert.fn8.iToc)
			return convert.fn8.stype;
		
		return 0;
	}
}

// new stuff for vscript so not only 1 func param?

#define	FUNC_TEMPLATE_FUNC_PARAMS_0
#define	FUNC_TEMPLATE_FUNC_PARAMS_1	, typename FUNC_ARG_TYPE_1

#define	SCRIPT_BINDING_ARGS_0
#define	SCRIPT_BINDING_ARGS_1 pArguments[0]

#define FUNC_BASE_TEMPLATE_FUNC_PARAMS_PASSTHRU_0
#define FUNC_BASE_TEMPLATE_FUNC_PARAMS_PASSTHRU_1 , FUNC_BASE_TEMPLATE_FUNC_PARAMS_1

#define	FUNC_BASE_TEMPLATE_FUNC_PARAMS_0
#define	FUNC_BASE_TEMPLATE_FUNC_PARAMS_1	FUNC_ARG_TYPE_1

#define	FUNC_APPEND_PARAMS_0
#define	FUNC_APPEND_PARAMS_1 pDesc->m_Parameters.Append(ScriptDeduceType(FUNC_ARG_TYPE_1));

#define DEFINE_NONMEMBER_FUNC_TYPE_DEDUCER(N) \
	template <typename FUNCTION_RETTYPE FUNC_TEMPLATE_FUNC_PARAMS_##N> \
	inline void ScriptDeduceFunctionSignature(ScriptFuncDescriptor_t *pDesc, FUNCTION_RETTYPE (*pfnProxied)(FUNC_BASE_TEMPLATE_FUNC_PARAMS_##N)) { \
		pDesc->m_ReturnType = ScriptDeduceType(FUNCTION_RETTYPE); \
		FUNC_APPEND_PARAMS_##N \
	}
#define DEFINE_MEMBER_FUNC_TYPE_DEDUCER(N) \
	template <typename OBJECT_TYPE_PTR, typename FUNCTION_CLASS, typename FUNCTION_RETTYPE FUNC_TEMPLATE_FUNC_PARAMS_##N> \
	inline void ScriptDeduceFunctionSignature(ScriptFuncDescriptor_t *pDesc, OBJECT_TYPE_PTR pObject, FUNCTION_RETTYPE (FUNCTION_CLASS::*pfnProxied)(FUNC_BASE_TEMPLATE_FUNC_PARAMS_##N)) { \
		pDesc->m_ReturnType = ScriptDeduceType(FUNCTION_RETTYPE); \
		FUNC_APPEND_PARAMS_##N \
	}
#define DEFINE_CONST_MEMBER_FUNC_TYPE_DEDUCER(N) \
	template <typename OBJECT_TYPE_PTR, typename FUNCTION_CLASS, typename FUNCTION_RETTYPE FUNC_TEMPLATE_FUNC_PARAMS_##N> \
	inline void ScriptDeduceFunctionSignature(ScriptFuncDescriptor_t *pDesc, OBJECT_TYPE_PTR pObject, FUNCTION_RETTYPE (FUNCTION_CLASS::*pfnProxied)(FUNC_BASE_TEMPLATE_FUNC_PARAMS_##N) const) { \
		pDesc->m_ReturnType = ScriptDeduceType(FUNCTION_RETTYPE); \
		FUNC_APPEND_PARAMS_##N \
	}

DEFINE_NONMEMBER_FUNC_TYPE_DEDUCER(0)
DEFINE_NONMEMBER_FUNC_TYPE_DEDUCER(1)
DEFINE_MEMBER_FUNC_TYPE_DEDUCER(0)
DEFINE_MEMBER_FUNC_TYPE_DEDUCER(1)
DEFINE_CONST_MEMBER_FUNC_TYPE_DEDUCER(0)
DEFINE_CONST_MEMBER_FUNC_TYPE_DEDUCER(1)

#define DEFINE_SCRIPT_BINDINGS(N) \
	template <typename FUNC_TYPE, typename FUNCTION_RETTYPE FUNC_TEMPLATE_FUNC_PARAMS_##N> \
	class CNonMemberScriptBinding##N { \
	public: \
 		static bool Call(ScriptFunctionBindingStorageType_t pFunction, void *pContext, ScriptVariant_t *pArguments, int nArguments, ScriptVariant_t *pReturn) { \
			if (nArguments != N || !pReturn || pContext)return false; \
			*pReturn = (ScriptConvertFreeFuncPtrFromVoid<FUNC_TYPE>(pFunction))(SCRIPT_BINDING_ARGS_##N); \
			if (pReturn->m_type == FIELD_VECTOR) pReturn->m_pVector = new Vector(*pReturn->m_pVector); \
 			return true; \
 		} \
	}; \
	template <typename FUNC_TYPE FUNC_TEMPLATE_FUNC_PARAMS_##N> \
	class CNonMemberScriptBinding##N<FUNC_TYPE, void FUNC_BASE_TEMPLATE_FUNC_PARAMS_PASSTHRU_##N> { \
	public: \
		static bool Call(ScriptFunctionBindingStorageType_t pFunction, void *pContext, ScriptVariant_t *pArguments, int nArguments, ScriptVariant_t *pReturn) { \
			if (nArguments != N || pReturn || pContext) return false; \
			(ScriptConvertFreeFuncPtrFromVoid<FUNC_TYPE>(pFunction))(SCRIPT_BINDING_ARGS_##N); \
			return true; \
		} \
	}; \
	template <class OBJECT_TYPE_PTR, typename FUNC_TYPE, typename FUNCTION_RETTYPE FUNC_TEMPLATE_FUNC_PARAMS_##N> \
	class CMemberScriptBinding##N { \
	public: \
 		static bool Call(ScriptFunctionBindingStorageType_t pFunction, void *pContext, ScriptVariant_t *pArguments, int nArguments, ScriptVariant_t *pReturn) { \
			if (nArguments != N || !pReturn || !pContext) return false; \
			*pReturn = (((OBJECT_TYPE_PTR)(pContext))->*ScriptConvertFuncPtrFromVoid<FUNC_TYPE>(pFunction))(SCRIPT_BINDING_ARGS_##N); \
			if (pReturn->m_type == FIELD_VECTOR) pReturn->m_pVector = new Vector(*pReturn->m_pVector); \
 			return true; \
 		} \
	}; \
	template <class OBJECT_TYPE_PTR, typename FUNC_TYPE FUNC_TEMPLATE_FUNC_PARAMS_##N> \
	class CMemberScriptBinding##N<OBJECT_TYPE_PTR, FUNC_TYPE, void FUNC_BASE_TEMPLATE_FUNC_PARAMS_PASSTHRU_##N> { \
	public: \
		static bool Call(ScriptFunctionBindingStorageType_t pFunction, void *pContext, ScriptVariant_t *pArguments, int nArguments, ScriptVariant_t *pReturn) { \
			if (nArguments != N || pReturn || !pContext) return false; \
			(((OBJECT_TYPE_PTR)(pContext))->*ScriptConvertFuncPtrFromVoid<FUNC_TYPE>(pFunction))(SCRIPT_BINDING_ARGS_##N); \
			return true; \
		} \
	}; \
	template <typename FUNCTION_RETTYPE FUNC_TEMPLATE_FUNC_PARAMS_##N> \
	inline ScriptBindingFunc_t ScriptCreateBinding(FUNCTION_RETTYPE (*pfnProxied)(FUNC_BASE_TEMPLATE_FUNC_PARAMS_##N )) { \
		typedef FUNCTION_RETTYPE (*Func_t)(FUNC_BASE_TEMPLATE_FUNC_PARAMS_##N); \
		return &CNonMemberScriptBinding##N<Func_t, FUNCTION_RETTYPE FUNC_BASE_TEMPLATE_FUNC_PARAMS_PASSTHRU_##N>::Call; \
	} \
	template <typename OBJECT_TYPE_PTR, typename FUNCTION_CLASS, typename FUNCTION_RETTYPE FUNC_TEMPLATE_FUNC_PARAMS_##N> \
		inline ScriptBindingFunc_t ScriptCreateBinding(OBJECT_TYPE_PTR pObject, FUNCTION_RETTYPE (FUNCTION_CLASS::*pfnProxied)(FUNC_BASE_TEMPLATE_FUNC_PARAMS_##N)) { \
		typedef FUNCTION_RETTYPE (FUNCTION_CLASS::*Func_t)(FUNC_BASE_TEMPLATE_FUNC_PARAMS_##N); \
		return &CMemberScriptBinding##N<OBJECT_TYPE_PTR, Func_t, FUNCTION_RETTYPE FUNC_BASE_TEMPLATE_FUNC_PARAMS_PASSTHRU_##N>::Call; \
	} \
	template <typename OBJECT_TYPE_PTR, typename FUNCTION_CLASS, typename FUNCTION_RETTYPE FUNC_TEMPLATE_FUNC_PARAMS_##N> \
		inline ScriptBindingFunc_t ScriptCreateBinding(OBJECT_TYPE_PTR pObject, FUNCTION_RETTYPE (FUNCTION_CLASS::*pfnProxied)(FUNC_BASE_TEMPLATE_FUNC_PARAMS_##N) const) { \
		typedef FUNCTION_RETTYPE (FUNCTION_CLASS::*Func_t)(FUNC_BASE_TEMPLATE_FUNC_PARAMS_##N); \
		return &CMemberScriptBinding##N<OBJECT_TYPE_PTR, Func_t, FUNCTION_RETTYPE FUNC_BASE_TEMPLATE_FUNC_PARAMS_PASSTHRU_##N>::Call; \
	}

DEFINE_SCRIPT_BINDINGS(0);
DEFINE_SCRIPT_BINDINGS(1);

#define ScriptInitFuncDescriptorNamed(pDesc, func, scriptName) { \
	(pDesc)->m_pszScriptName = scriptName; \
	(pDesc)->m_pszFunction = #func; \
	ScriptDeduceFunctionSignature(pDesc, &func); \
}

#define ScriptInitFunctionBindingNamed(pScriptFunction, func, scriptName) { \
	ScriptInitFuncDescriptorNamed((&(pScriptFunction)->m_desc), func, scriptName); \
	(pScriptFunction)->m_pfnBinding = ScriptCreateBinding(&func); \
	(pScriptFunction)->m_pFunction = ScriptConvertFreeFuncPtrToVoid(&func); \
}

#define ScriptRegisterFunctionNamed(pVM, func, scriptName, description) { \
	static ScriptFunctionBinding_t binding; \
	binding.m_desc.m_pszDescription = description; \
	binding.m_desc.m_Parameters.Clear(); \
	ScriptInitFunctionBindingNamed(&binding, func, scriptName); \
	pVM->RegisterFunction(&binding); \
}

#define ScriptRegisterFunction(pVM, func, description) ScriptRegisterFunctionNamed(pVM, func, #func, description)

// -- Tier1 command stuff --

struct ConCommandBase;
struct CCommand;
typedef void (*FnChangeCallback_t)(void* var, const char* pOldValue, float flOldValue);

#define COMMAND_COMPLETION_MAXITEMS 64
#define COMMAND_COMPLETION_ITEM_LENGTH 64

using _CommandCallback = void (*)(const CCommand& args);
using _CommandCompletionCallback = int (*)(const char* partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH]);
using _InternalSetValue = void(__rescalll*)(void* thisptr, const char* value);
using _InternalSetFloatValue = void(__rescalll*)(void* thisptr, float value);
using _InternalSetIntValue = void(__rescalll*)(void* thisptr, int value);
using _RegisterConCommand = void(__rescalll*)(void* thisptr, ConCommandBase* pCommandBase);
using _UnregisterConCommand = void(__rescalll*)(void* thisptr, ConCommandBase* pCommandBase);
using _FindCommandBase = void*(__rescalll*)(void* thisptr, const char* name);
using _InstallGlobalChangeCallback = void(__rescalll*)(void* thisptr, FnChangeCallback_t callback);
using _RemoveGlobalChangeCallback = void(__rescalll*)(void* thisptr, FnChangeCallback_t callback);
using _AutoCompletionFunc = int(__rescalll*)(void* thisptr, char const* partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH]);

struct ConCommandBase {
	void* ConCommandBase_VTable;
	ConCommandBase* m_pNext;
	bool m_bRegistered;
	const char* m_pszName;
	const char* m_pszHelpString;
	int m_nFlags;

	ConCommandBase(const char* name, int flags, const char* helpstr) :
		ConCommandBase_VTable(nullptr),
		m_pNext(nullptr),
		m_bRegistered(nullptr),
		m_pszName(name),
		m_pszHelpString(helpstr),
		m_nFlags(flags)
	{}
};

struct CCommand {
	enum {
		COMMAND_MAX_ARGC = 64,
		COMMAND_MAX_LENGTH = 512
	};
	int m_nArgc;
	int m_nArgv0Size;
	char m_pArgSBuffer[COMMAND_MAX_LENGTH];
	char m_pArgvBuffer[COMMAND_MAX_LENGTH];
	const char* m_ppArgv[COMMAND_MAX_ARGC];

	int ArgC() const {
		return this->m_nArgc;
	}
	const char* Arg(int nIndex) const {
		return this->m_ppArgv[nIndex];
	}
	const char* operator[](int nIndex) const {
		return Arg(nIndex);
	}
};

struct ConCommand : ConCommandBase {
	union {
		void* m_fnCommandCallbackV1;
		_CommandCallback m_fnCommandCallback;
		void* m_pCommandCallback;
	};

	union {
		_CommandCompletionCallback m_fnCompletionCallback;
		void* m_pCommandCompletionCallback;
	};

	bool m_bHasCompletionCallback : 1;
	bool m_bUsingNewCommandCallback : 1;
	bool m_bUsingCommandCallbackInterface : 1;

	ConCommand(const char* pName, _CommandCallback callback, const char* pHelpString, int flags, _CommandCompletionCallback completionFunc) :
		ConCommandBase(pName, flags, pHelpString),
		m_fnCommandCallback(callback),
		m_fnCompletionCallback(completionFunc),
		m_bHasCompletionCallback(completionFunc != nullptr),
		m_bUsingNewCommandCallback(true),
		m_bUsingCommandCallbackInterface(false)
	{}
};

struct ConVar : ConCommandBase {
	void* ConVar_VTable;
	ConVar* m_pParent;
	const char* m_pszDefaultValue;
	char* m_pszString;
	int m_StringLength;
	float m_fValue;
	int m_nValue;
	bool m_bHasMin;
	float m_fMinVal;
	bool m_bHasMax;
	float m_fMaxVal;
	CUtlVector<FnChangeCallback_t> m_fnChangeCallback;

	ConVar(const char* name, const char* value, int flags, const char* helpstr, bool hasmin, float min, bool hasmax, float max) :
		ConCommandBase(name, flags, helpstr),
		ConVar_VTable(nullptr),
		m_pParent(nullptr),
		m_pszDefaultValue(value),
		m_pszString(nullptr),
		m_fValue(0.0f),
		m_nValue(0),
		m_bHasMin(hasmin),
		m_fMinVal(min),
		m_bHasMax(hasmax),
		m_fMaxVal(max),
		m_fnChangeCallback()
	{}
};

// -- Client chat --

class bf_read {
public:
	uint32_t ReadUnsigned(int nbits) {
		if(m_nBitsAvail >= nbits) {
			unsigned val = m_nInBufWord & ((1 << nbits) - 1);
			m_nBitsAvail -= nbits;
			m_nInBufWord >>= nbits;
			if(m_nBitsAvail == 0) {
				m_nBitsAvail = 32;
				m_nInBufWord = *(m_pDataIn++);
			}
			return val;
		}
		int rem = nbits - m_nBitsAvail;
		uint32_t hi = m_nInBufWord << rem;
		m_nBitsAvail = 32;
		m_nInBufWord = *(m_pDataIn++);
		uint32_t lo = m_nInBufWord & ((1 << rem) - 1);
		return (hi | lo) & ((1 << nbits) - 1);
	}
	const char *m_pDebugName;
	bool m_bOverflow;
	int m_nDataBits;
	size_t m_nDataBytes;
	uint32_t m_nInBufWord;
	int m_nBitsAvail;
	const uint32_t *m_pDataIn;
	const uint32_t *m_pBufferEnd;
	const uint32_t *m_pData;
};