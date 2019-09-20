
#include <iostream>
#include <unordered_set>
#include <vector>
#include <queue>
#include <unordered_map>
#include "wikiscraper.h"

#include <fstream>
#include <sstream>
//#include <algorithm>


using std::cout;            using std::endl;
using std::string;          using std::vector;
using std::priority_queue;  using std::unordered_map;
using std::unordered_set;

using std::ifstream;
using std::stringstream;
using std::find;
using std::search;
using std::all_of;
/*
 * This is the function you will be implementing. It takes
 * two string representing the names of a start_page and
 * end_page and is supposed to return a ladder, represented
 * as a vector<string>, of links that can be followed from
 * start_page to get to the end_page.
 *
 * For the purposes of this algorithm, the "name" of a Wikipedia
 * page is what shows at the end of the URL when you visit that page
 * in your web browser. For ex. the name of the Stanford University
 * Wikipedia page is "Stanford_University" since the URL that shows
 * in your browser when you visit this page is:
 *
 *       https://en.wikipedia.org/wiki/Stanford_University
 */
vector<string> findWikiLadder(const string& start_page, const string& end_page) {
    // TODO: Fill in the findWikiLinks method in wikiscraper.cpp,
    //       then write this  function as per the handout.
    //
    //                Best of luck!
    vector<string> ret;
    string html_page;
    stringstream str_stream;
    ifstream html_file("/Users/hernl229/Documents/CS106L/cs106l/02_WikiRacerLinksA/WikiRacerLinks/res/simple.txt");
    str_stream << html_file.rdbuf();
    html_page = str_stream.str();
    string wiki_start= string("<a");
    string wiki_end = string("</a>");

    string::iterator page_start = html_page.begin();
    string::iterator page_end = html_page.end();
    string::iterator link_start = html_page.begin();
    string::iterator link_end;

    while(page_start != page_end){
        //Extract link
        link_start = search(page_start, page_end,
                        wiki_start.begin(), wiki_start.end());
        //Page will start at the end of link
        page_start = search(link_start, page_end, wiki_end.begin(), wiki_end.end());

        if(page_start != page_end){

            //Create substr until last character </a>
            page_start += 4;
            string link = string(link_start, page_start);

            //Search PAGENAME
            string wiki_str_start = string("/wiki/");
            string wiki_str_end = {"\""};
            link_start = search(link.begin(), link.end(),
                            wiki_str_start.begin(), wiki_str_start.end());
            link_end = search(link_start, link.end(),
                            wiki_str_end.begin(), wiki_str_end.end());
            if(link_start != link.end()){
                //offset by 6 (/wiki/)
                string pagename = string(link_start+6, link_end);

                if(!all_of(pagename.begin(), pagename.end(), [](char c){return c == ':';})){
                    cout << "pagename: " << pagename << endl;
                    ret.push_back(pagename);
                }
            }
        }
    }

    cout << "Done!" << endl;

    return ret;
}

int main() {
    auto ladder = findWikiLadder("Fruit", "Strawberry");
    cout << endl;

    if(ladder.empty()) {
        cout << "No ladder found!" << endl;
    } else {
        cout << "Ladder found:" << endl;
        cout << "\t";

        // Print the ladder here!
    }

    return 0;
}




