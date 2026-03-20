#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include <miniaudio.h>
#include <unordered_map>
#include <string>

#include "../Mxm/Vec3.h"

class AudioSystem final
{
private:
	struct AudioData
	{
		ma_sound sound;
		bool _repeat = false;
	};

	ma_engine _engine;
	std::unordered_map<std::string, AudioData> _sounds;

	AudioSystem();
	~AudioSystem();
public:
	AudioSystem(const AudioSystem&) = delete;
	AudioSystem& operator=(const AudioSystem&) = delete;
	AudioSystem(AudioSystem&&) = delete;
	AudioSystem& operator=(AudioSystem&&) = delete;

	static AudioSystem& getInstance() noexcept {
		static AudioSystem manager;
		return manager;
	}

	bool loadSoundFromFile(const std::string& name, const std::string& path, bool repeat = false);
	void playSound(const std::string& name);
	void playSound3D(const std::string& name, const Mxm::Vec3& position);
	void stopSound(const std::string& name);

	void setMinMaxDist(const std::string& name, float min, float max);
	void setSoundPosition(const std::string& name, const Mxm::Vec3& position);
	void setListener(const Mxm::Vec3& position, const Mxm::Vec3& forward);

	bool isSoundPlaying(const std::string& name) const noexcept;

	void setVolume(const std::string& name, float volume);
	void setRepeat(const std::string& name, bool repeat);
};

#endif // !AUDIOMANAGER_H
