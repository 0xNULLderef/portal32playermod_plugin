// #include <client.hpp>
// #include <offsets.hpp>

// Client::Client() {}

// bool Client::Init() {
// 	this->g_ClientTools = Interface::Create(this->Name(), "VCLIENTTOOLS001", false);
// 	if(this->g_ClientTools) {
// 		this->NextParticleSystem = this->g_ClientTools->Original<_NextParticleSystem>(Offsets::NextParticleSystem);
// 	}

// 	return this->g_ClientTools && this->NextParticleSystem;
// }

// void Client::Shutdown() {
// 	this->NextParticleSystem = nullptr;
// 	Interface::Delete(this->g_ClientTools);
// }