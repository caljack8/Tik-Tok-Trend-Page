#include "Video.h"

//Constructor
Video::Video(unsigned long long int aview_num, const std::string& acoverURL, const std::string& awebURL){
	view_num = aview_num;
	coverURL = acoverURL;
	webURL = awebURL;
}