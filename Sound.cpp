#include "Sound.h"

//constructor
Sound::Sound(const std::string& amusic_id, const std::string& anauthor, const std::string& asound_name){

	sound_name = asound_name;
	music_id = amusic_id;
	author = anauthor;
	views = 0;
}