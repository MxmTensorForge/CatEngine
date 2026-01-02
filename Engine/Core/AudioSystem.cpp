#include "AudioSystem.h"
#include "Logger.h"

AudioSystem::AudioSystem() {
	ma_result result = ma_engine_init(nullptr, &_engine);
	if (result != MA_SUCCESS) {
		Logger::getInstance().log(LogType::Fatal, "Audio engine failed");
	}
}
AudioSystem::~AudioSystem() {
	for (auto& [name, data] : _sounds)
		ma_sound_uninit(&data.sound);
	ma_engine_uninit(&_engine);
}

bool AudioSystem::loadSound(const std::string& name, const std::string& path, bool repeat) {
	auto [it, inserted] = _sounds.try_emplace(name);
	if (!inserted) {
		return false;
	}

	AudioData& data = it->second;
	ma_result result = ma_sound_init_from_file(&_engine, path.c_str(), 0, nullptr, nullptr, &data.sound);

	if (result != MA_SUCCESS) {
		_sounds.erase(it);
		Logger::getInstance().log(LogType::Fatal, "Audio sound failed");
		return false;
	}

	data._repeat = repeat;
	ma_sound_set_looping(&data.sound, repeat);

	Logger::getInstance().log(LogType::Message, "Sound loaded successfully: " + name);
	return true;
}
void AudioSystem::playSound(const std::string& name) {
	auto it = _sounds.find(name);
	if (it != _sounds.end()) {
		ma_sound_seek_to_pcm_frame(&it->second.sound, 0);
		ma_sound_start(&it->second.sound);
	}
}
void AudioSystem::stopSound(const std::string& name) {
	auto it = _sounds.find(name);
	if (it != _sounds.end()) {
		ma_sound_stop(&it->second.sound);
	}
}

void AudioSystem::setVolume(const std::string& name, float volume) {
	auto it = _sounds.find(name);
	if (it != _sounds.end()) {
		ma_sound_set_volume(&it->second.sound, volume);
	}
}
void AudioSystem::setRepeat(const std::string& name, bool repeat) {
	auto it = _sounds.find(name);
	if (it != _sounds.end()) {
		ma_sound_set_looping(&it->second.sound, repeat);
	}
}