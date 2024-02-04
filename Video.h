#ifndef Video_h
#define Video_h
#include <string>

class Video{

public:

	//constructor
	Video(unsigned long long int aview_num, const std::string& acoverURL, const std::string& awebURL);

	//accessors
	const unsigned long long int get_views() const {return view_num;}
	const std::string& get_cover() const {return coverURL;}
	const std::string& get_web() const {return webURL;}

private:

	//private member variables
	unsigned long long int view_num;
	std::string coverURL;
	std::string webURL;

};

//functor for comparing video pointers
class compare_video{
public:
	bool operator()(const Video* v1, const Video* v2){
		return v1->get_views() < v2->get_views();
	}
};

#endif