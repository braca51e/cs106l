#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include "wikiscraper.h"
#include "error.h"

using std::cout;            using std::endl;
using std::cerr;            using std::string;
using std::unordered_map;   using std::unordered_set;
using std::search;

/*
 * You should delete the code in this function and
 * fill it in with your code from part A of the assignment.
 *
 * If you used any helper functions, just put them above this function.
 */
unordered_set<string> findWikiLinks(const string& inp) {
    // For now will read txt directly from disk

    unordered_set<string> ret;
    string wiki_start= string("<a");
    string wiki_end = string("</a>");

    string::const_iterator page_start = inp.begin();
    string::const_iterator page_end = inp.end();
    string::const_iterator link_start = inp.begin();
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
                    ret.insert(pagename);
                }
            }
        }
    }

    cout << "Done!" << endl;

    return ret;
}


/*
 * ==================================================================================
 * |                           DON"T EDIT ANYTHING BELOW HERE                       |
 * ==================================================================================
 */
unordered_set<string> WikiScraper::getLinkSet(const string& page_name) {
    if(linkset_cache.find(page_name) == linkset_cache.end()) {
        auto links = findWikiLinks(getPageSource(page_name));
        linkset_cache[page_name] = links;
    }
    return linkset_cache[page_name];
}


WikiScraper::WikiScraper() {
    (void)getPageSource("Main_Page");
#ifdef _WIN32
    // define something for Windows
    system("cls");
#else
    // define it for a Unix machine
    system("clear");
#endif
}


string createPageUrl(const string& page_name) {
    return "https://en.wikipedia.org/wiki/" + page_name;
}

void notFoundError(const string& msg, const string& page_name, const string& url) {
    const string title = "    AN ERROR OCCURED DURING EXECUTION.    ";
    const string border(title.size() + 4, '*');
    cerr << endl;
    errorPrint(border);
    errorPrint("* " + title + " *");
    errorPrint(border);
    errorPrint();
    errorPrint("Reason: " + msg);
    errorPrint();
    errorPrint("Debug Information:");
    errorPrint();
    errorPrint("\t- Input parameter: " + page_name);
    errorPrint("\t- Attempted url: " + url);
    errorPrint();
}

string WikiScraper::getPageSource(const string &page_name) {
    const static string not_found = "Wikipedia does not have an article with this exact name.";
    if(page_cache.find(page_name) == page_cache.end()) {
        QUrl url(createPageUrl(page_name).c_str()); // need c string to convert to QString

        QNetworkRequest request(url);

        QNetworkReply *reply(manager.get(request));

        QEventLoop loop;
        QObject::connect(reply, SIGNAL(finished()) , &loop, SLOT(quit()));
        loop.exec();

        string ret = reply->readAll().toStdString();
        if(std::search(ret.begin(), ret.end(), not_found.begin(), not_found.end()) != ret.end()){
            notFoundError("Page does not exist!", page_name, url.toString().toStdString());
            return "";
        }
        size_t indx = ret.find("plainlinks hlist navbar mini");
        if(indx != string::npos) {
            return ret.substr(0, indx);
        }
        page_cache[page_name] = ret;
    }
    return page_cache[page_name];
}





