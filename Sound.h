#ifndef Sound_h
#define Sound_h
#include <queue>
#include "Video.h"

class Sound{

public:

	//constructor
	Sound(const std::string& amusic_id, const std::string& anauthor, const std::string& asound_name);

	//accesors
	unsigned long long int get_views() {return views;}
	const std::priority_queue<Video*, std::vector<Video*>, compare_video>& get_videos() const {return top3;}
	const std::string get_id() const {return music_id;}
	const std::string get_author() const {return author;}
	const std::string get_name() const {return sound_name;}

	//adders
	void add_views(unsigned long long int view_num) {views+=view_num;}
	void add_video(Video* vid) {top3.push(vid);}
	void remove_video() {top3.pop();}

private:

	//private variables
	unsigned long long int views;
	std::priority_queue<Video*, std::vector<Video*>, compare_video> top3;
	std::string music_id;
	std::string author;
	std::string sound_name;
};

#endif