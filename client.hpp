#include <sdk.hpp>
#include <utils.hpp>
#include <interface.hpp>

class Client {
public:
	Interface* g_ClientTools;

	using _NextParticleSystem = void*(__func*)(void* thisptr, void* searchResult);
	_NextParticleSystem NextParticleSystem = nullptr;

	CNewParticleEffect* GetParticleSystem(CNewParticleEffect* prev);
	inline Vector GetPortalGunIndicatorColor() const { return this->portalGunIndicatorColor; }
	void SetPortalGunIndicatorColor(Vector v);
	void UpdatePortalGunIndicatorColor();

public:
	Client();
	bool Init();
	void Shutdown();
	const char* Name() { return MODULE("client"); }

private:
	Vector portalGunIndicatorColor;
};

extern Client* engine;