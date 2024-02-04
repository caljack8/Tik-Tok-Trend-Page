#ifndef Hashtag_h
#define Hashtag_h
#include <queue>
#include "Video.h"

class Hashtag{

public:

	//constructor
	Hashtag(const std::string& ahash_name);

	//accesors
	unsigned long long int get_views() {return views;}
	unsigned long long int get_used() {return used;}
	const std::string get_name() const {return hash_name;}
	const std::priority_queue<Video*, std::vector<Video*>, compare_video>& get_videos() const {return top3;}

	//adders
	void add_views(unsigned long long int view_num){views+=view_num;}
	void add_used(){used++;}
	void add_video(Video* vid) {top3.push(vid);}
	void remove_video() {top3.pop();}

private:

	//private memeber variables
	unsigned long long int views;
	unsigned long long int used;
	std::string hash_name;
	std::priority_queue<Video*, std::vector<Video*>, compare_video> top3;
};

#endif