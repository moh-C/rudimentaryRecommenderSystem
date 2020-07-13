#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <typeinfo>

using namespace std;

#define MOVIES_LENGTH 60
#define USERS_LENGTH 50

float rating[USERS_LENGTH][MOVIES_LENGTH] = {};

struct movie {
    int index;
    string name;
    int year;
    string yearDetail;
    string link;
    string genre;
} movies[MOVIES_LENGTH];

movie processor(vector<string>);

void load_movies();
void load_rating_mat();

int main() {
    load_rating_mat();
    load_movies();
}

/**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**/

movie moviesProcessor(vector<string> details){
    movie temp;
    int index_ = 0, year_ = 0;

    string s1 = details[0], s2 = details[2];
    stringstream stoi_1(s1), stoi_2(s2);

    stoi_1 >> index_;
    stoi_2 >> year_;

    temp.index = index_;
    temp.year = year_;
    temp.name = details[1];
    temp.yearDetail = details[3];
    temp.link = details[4];
    temp.genre = details[5];

    return temp;
}

void load_movies(){
    ifstream file("Movies.txt");
    string line;
    string partial;

    vector<string> tokens;
    int index = 0;

    while(getline(file, line)) {
        istringstream iss(line);
        string token;
        while(getline(iss, token, '\t'))
            tokens.push_back(token);
        movies[index++] = moviesProcessor(tokens);
        if(index == MOVIES_LENGTH) break;
        tokens.clear();
    }
}

void load_rating_mat(){
    ifstream file("Ratings.txt");
    string line;
    string partial;

    vector<string> tokens;

    while(getline(file, line)) {
        istringstream iss(line);
        string token;
        while(getline(iss, token, '\t'))
            tokens.push_back(token);

        int index0_ = 0, index1_ = 0, index2_ = 0;

        string s0 = tokens[0], s1 = tokens[1], s2 = tokens[2];
        stringstream stoi_0(s0), stoi_1(s1), stoi_2(s2);

        stoi_0 >> index0_, stoi_1 >> index1_, stoi_2 >> index2_;
        rating[index0_ - 1][index1_ - 1] = index2_;

        tokens.clear();
    }
}
