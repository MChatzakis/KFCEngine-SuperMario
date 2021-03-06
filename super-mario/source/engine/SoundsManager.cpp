#include "./SoundsManager.h"
#include <iostream>

//Sound
Sound::Sound(){}

Sound::Sound(std::string _id, ALLEGRO_SAMPLE* _sample, std::string _sample_path, float _gain, float _pan, float _speed, ALLEGRO_PLAYMODE _loop)
	: id(_id), sample(_sample), sample_path(_sample_path), gain(_gain), pan(_pan), speed(_speed), loop(_loop)
{
	sampleId._id = 0;
	//sampleId._index = 0;
}

std::string Sound::toString() {
	
	return "{Id: " + id + ",Sample: " + std::to_string((int)sample) + ",Sample Path: " + sample_path + ",Playmode: " + std::to_string((int)loop) + ",Speed: " + std::to_string(speed) + ",Gain: " + std::to_string(gain) + ",Pan: " + std::to_string(pan) + ",SampleId: " + std::to_string(sampleId._id) + "}";
}

//Sounds Manager
SoundsManager SoundsManager::singleton;

SoundsManager::SoundsManager(void) {}

SoundsManager::~SoundsManager() { CleanUp(); }

SoundsManager& SoundsManager::GetSingleton()
{
	return singleton;
}

auto SoundsManager::Get(void) -> const SoundsManager& { return singleton; }

void SoundsManager::PlaySound(std::string id) {
	Sound* sound = GetSound(id);
	if (sound == nullptr) {
		return;
	}

	ALLEGRO_SAMPLE_ID retId;
	al_play_sample(sound->sample, sound->gain, sound->pan, sound->speed, sound->loop, &retId);
	sound->sampleId = retId;
}

void SoundsManager::StopSound(std::string id) {
	Sound* sound = GetSound(id);
	if (sound == nullptr) {
		return;
	}

	al_stop_sample(&(sound->sampleId));
}

void SoundsManager::CleanUp()
{
	for (auto& i : sounds) {
		delete(i.second);
	}
	sounds.clear();
}

Sound* SoundsManager::GetSound(std::string id) {
	auto i = sounds.find(id);
	return i != sounds.end() ? i->second : nullptr;
}

void SoundsManager::LoadSounds(std::string filename, const SoundsManager::Parser& parser)
{
	std::list<Sound> output;
	auto result = parser(output, filename);
	assert(result);

	for (auto& entry : output) {
		assert(!GetSound(entry.id));
		sounds[entry.id] = new Sound(entry.id, al_load_sample(entry.sample_path.c_str()), entry.sample_path, entry.gain, entry.pan, entry.speed, entry.loop);
	}

	return;
}

void SoundsManager::PrintSoundsList() {
	for (std::map<std::string, Sound*>::const_iterator it = sounds.begin(); it != sounds.end(); ++it) {
		//std::cout << it->first << " " << it->second.first << " " << it->second.second << "\n";
		std::cout << "{" << it->first << " : " << it->second->toString() << "}\n";
	}
}

