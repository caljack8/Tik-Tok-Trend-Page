#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <queue>
#include <regex>
#include <algorithm>
#include "Video.h"
#include "Hashtag.h"
#include "Sound.h"

//functor to sort the hashtags
class sort_hashtags{
public:
    bool operator()(Hashtag* h1, Hashtag* h2){

        //if the used count and view count are equal cimpare the names
        if((h1->get_used() == h2->get_used()) && (h1->get_views() == h2->get_views())){
            return h1->get_name() > h2->get_name();
        }

        //if onlly the used count is equal compare the views
        else if(h1->get_used() == h2->get_used()){
            return h1->get_views() < h2->get_views();
        }

        //otherwise compare the use count
        else{
            return h1->get_used() < h2->get_used();
        }
    }
};

//functor to sort the sounds
class sort_sounds{
public:
    bool operator()(Sound* s1, Sound* s2){

        //if the views are th same compare the music id
        if(s1->get_views() == s2->get_views()){
            return s1->get_id() > s2->get_id();
        }

        //otherwise compare using the views
        else{
            return s1->get_views() < s2->get_views();
        }
    }
};

//function to print out the sounds
void print_sound(Sound* s1, std::ofstream& ofile){

    //print out the sounds information
    ofile << "========================" << std::endl;
    ofile << s1->get_name() << std::endl;
    ofile << s1->get_views() << " views" << std::endl;
    ofile << s1->get_author() << std::endl;
    ofile << "music id: " << s1->get_id() << "\n" << std::endl;

    //print out the top videos associated with this sound
   //print out the top3 videos
    int vid_count = 0;
    while(!s1->get_videos().empty() && vid_count < 3){
        ofile << "cover url: " << s1->get_videos().top()->get_cover() << std::endl;
        ofile << "web video url: " << s1->get_videos().top()->get_web() << std::endl;
        s1->remove_video();
        vid_count++;
    }
    ofile << "========================" << std::endl;
}

void print_hashtag(Hashtag* h1, std::ofstream& ofile){

    //print out the hashtag information
    ofile << "========================" << std::endl;
    ofile << "#" << h1->get_name() << std::endl;
    ofile << "used " << h1->get_used() << " times" << std::endl;
    ofile << h1->get_views() << " views" << "\n" << std::endl;

    //print out the top3 videos
    int vid_count = 0;
    while(!h1->get_videos().empty() && vid_count < 3){
        ofile << "cover url: " << h1->get_videos().top()->get_cover() << std::endl;
        ofile << "web video url: " << h1->get_videos().top()->get_web() << std::endl;
        h1->remove_video();
        vid_count++;
    }
    ofile << "========================" << std::endl;
}


int main(int argc, char* argv[]){

	//check the correct amount of arguments
	if(argc != 4 ){
		std::cerr << "Not the correct amount of arguments, Goodbye." << std::endl;
		exit(1);
	}

	std::ofstream ofile(argv[2]);
	//checks if the out file opened correctly
  	try{
        if(!ofile.good()) { 
    	   throw std::string("Failed to open ");
        }
    }

    catch(std::string& error){
        std::cerr << error << argv[2] << std::endl;
        exit(1);
    }

    std::ifstream jsonFile(argv[1]);
	//checks if the input file opened correctly
    try{
        if(!jsonFile.good()) { 
           throw std::string("Failed to open ");
        }
    }

    catch(std::string& error){
        std::cerr << error << argv[1] << std::endl;
        exit(1);
    }

    //put the command into a string
    std::string command = argv[3];

    //reads in the json file into a string
    std::string json_content;
    std::string line;
    while (std::getline(jsonFile, line)) {
            json_content += line;
    }
    // don't need this json file anymore, as the content is read into json_content.
    jsonFile.close();

    //declares the variables for the video class and their start positions
    unsigned long int vid_views;
    std::string coverURL;
    std::string webURL;
    unsigned long int vid_views_pos = 0;
    unsigned long int coverURL_pos = 0;
    unsigned long int webURL_pos = 0;


    //get the top sounds
    if(command == "sound"){

        //hash map of all sounds
        std::unordered_map<std::string, Sound*> all_sounds;

        //declares the variables for the sound class
        std::string music_id;
        std::string music_author;
        std::string sound_name;

        //declare the start positions for the sound variables
        unsigned long int music_id_pos = 0;
        unsigned long int music_author_pos = 0;
        unsigned long int sound_name_pos = 0;

        while(json_content.find("id", vid_views_pos) != std::string::npos){

            //get the starting positions for each string 
            sound_name_pos = json_content.find("\"musicName\": \"", sound_name_pos)+14;
            music_id_pos = json_content.find("\"musicId\": \"", music_id_pos)+12;
            music_author_pos = json_content.find("\"musicAuthor\": \"", vid_views_pos)+16;
            vid_views_pos = json_content.find("\"playCount\": ", vid_views_pos)+13;
            coverURL_pos = json_content.find("\"coverUrl\": \"", coverURL_pos)+13;
            webURL_pos = json_content.find("\"webVideoUrl\": \"", webURL_pos)+16;


            //get the string for each compnenet of the comment
            sound_name = json_content.substr(sound_name_pos, json_content.find("\"", sound_name_pos) - sound_name_pos);
            music_id = json_content.substr(music_id_pos, json_content.find("\"", music_id_pos) - music_id_pos);
            music_author = json_content.substr(music_author_pos, json_content.find("\"", music_author_pos) - music_author_pos);
            vid_views = stoi(json_content.substr(vid_views_pos, json_content.find(",", vid_views_pos) - vid_views_pos));
            coverURL = json_content.substr(coverURL_pos, json_content.find("\"", coverURL_pos) - coverURL_pos);
            webURL = json_content.substr(webURL_pos, json_content.find("\"", webURL_pos) - webURL_pos);

            //create the video object for this video
            Video* video = new Video(vid_views, coverURL, webURL);


            //check if the sound is in the hash map already
            if(all_sounds.find(music_id) != all_sounds.end()){
                all_sounds[music_id]->add_views(vid_views);
            }

            //otherwise make the sound and add it to the hash map
            else{

                Sound* sound = new Sound(music_id, music_author, sound_name);
                all_sounds[music_id] = sound;
                all_sounds[music_id]->add_views(vid_views);
            }

            //add the video if it's in the top3 views
            all_sounds[music_id]->add_video(video);

        }

        //create the priority queue for the sounds
        std::priority_queue<Sound*, std::vector<Sound*>, sort_sounds> sound_queue;

        //add all the sounds to the priority queue and delete them from the map to save memory
        std::unordered_map<std::string, Sound*>::iterator sound_itr = all_sounds.begin();
        while(sound_itr != all_sounds.end()){
            sound_queue.push(sound_itr->second);
            sound_itr = all_sounds.erase(sound_itr);
        }

        //prints the top 10 sounds
        ofile << "trending sounds:\n" << std::endl; 
        for(int i = 0; i < 10; i++){
            print_sound(sound_queue.top(), ofile);
            sound_queue.pop();
        }

        return 0;
    }

    //get the top 10 hashtags
    else{

        //hash map of all hashtags
    	std::unordered_map<std::string, Hashtag*> all_hashtags;

        //declare the variable for the hashtag class
        std::string text;

        //declare the start position for the hashtag variable
        unsigned long int text_pos = 0;

        //get all the info from the json file
        while(json_content.find("text", text_pos) != std::string::npos){

            //get the starting positions for each string
            text_pos = json_content.find("\"text\": \"", text_pos)+9;
            vid_views_pos = json_content.find("\"playCount\": ", vid_views_pos)+13;
            coverURL_pos = json_content.find("\"coverUrl\": \"", coverURL_pos)+13;
            webURL_pos = json_content.find("\"webVideoUrl\": \"", webURL_pos)+16;            

            //json_content.find("\"createTime",text_pos)
            //get the string for each component of the comment
            text = json_content.substr(text_pos, json_content.find("\"", text_pos) - text_pos);
            vid_views = std::stol(json_content.substr(vid_views_pos, json_content.find(",", vid_views_pos) - vid_views_pos));
            coverURL = json_content.substr(coverURL_pos, json_content.find("\"", coverURL_pos) - coverURL_pos);
            webURL = json_content.substr(webURL_pos, json_content.find("\"", webURL_pos) - webURL_pos);

            //create the video object for this video
            Video* video = new Video(vid_views, coverURL, webURL);

            //go through the text and get each hashtag and either add
            // define a regular expression to match hashtags
            std::regex hashtagRegex("#([\\w\\u0080-\\uFFFF]+)");

            // create an iterator for matching
            std::sregex_iterator hashtagIterator(text.begin(), text.end(), hashtagRegex);
            std::sregex_iterator endIterator;

            // iterate over the matches and extract the hashtags
            while (hashtagIterator != endIterator) {
                std::smatch match = *hashtagIterator;
                std::string hash_name = match.str(1);  // extract the first capturing group

                //check if the hashtag is in the hash map already
                if(all_hashtags.find(hash_name) != all_hashtags.end()){
                    all_hashtags[hash_name]->add_views(vid_views);
                    all_hashtags[hash_name]->add_used();
                }

                //otherwse make the hashtag and add it to the hash map
                else{
                    Hashtag* hashtag  = new Hashtag(hash_name);
                    all_hashtags[hash_name] = hashtag;
                    all_hashtags[hash_name]->add_views(vid_views);
                    all_hashtags[hash_name]->add_used();
                }

                //add the video to the priority queue in the hashtag object
                all_hashtags[hash_name]->add_video(video);

                ++hashtagIterator;
            }
        }


        //create the priority queue for the Hashtags
        std::priority_queue<Hashtag*, std::vector<Hashtag*>, sort_hashtags> hashtag_queue;

        //add all the sounds to the priority queue and delete them from the map to save memory
        std::unordered_map<std::string, Hashtag*>::iterator hashtag_itr = all_hashtags.begin();
        while(hashtag_itr != all_hashtags.end()){
            hashtag_queue.push(hashtag_itr->second);
            hashtag_itr = all_hashtags.erase(hashtag_itr);
        }

        //prints the top 10 sounds
        ofile << "trending hashtags:\n" << std::endl; 
        for(int i = 0; i < 10; i++){
            print_hashtag(hashtag_queue.top(), ofile);
            hashtag_queue.pop();
        }
    }

    return 0;
}