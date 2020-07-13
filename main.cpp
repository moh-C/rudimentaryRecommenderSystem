#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <cmath>

using namespace std;

#define MOVIES_LENGTH 60
#define USERS_LENGTH 50

float similarity_all[USERS_LENGTH][MOVIES_LENGTH];

struct movie {
    int ID;
    string name;
    int year;
    string yearDetail;
    string link;
    string genre;
    float rating[50] = { 0.0 };
};

vector<movie> movies;

movie processor(vector<string>);

void load_movies();
void load_rating_mat();
void calc_similarity();
float similarity(int, int);

int main() {
    load_movies();
    load_rating_mat();
    calc_similarity();

    for(int i=0;i<50;i++){
        cout << similarity_all[i][i] << "\n";
    }
}

/*------------------------------------------------------------------------------------------------------------------------------*/

movie moviesProcessor(vector<string> details){
    movie temp;
    int index_ = 0, year_ = 0;

    string s1 = details[0], s2 = details[2];
    stringstream stoi_1(s1), stoi_2(s2);

    stoi_1 >> index_;
    stoi_2 >> year_;

    temp.ID = index_;
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

    vector<string> tokens;

    while(getline(file, line)) {
        istringstream iss(line);
        string token;
        while(getline(iss, token, '\t'))
            tokens.push_back(token);
        movies.push_back(moviesProcessor(tokens));
        tokens.clear();
    }
}

void load_rating_mat(){
    ifstream file("Ratings.txt");
    string line;

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

        movies[index1_ - 1].rating[index0_ - 1] = index2_;
        if (index2_ > 20) cout << index2_;
        tokens.clear();
    }
}

float similarity(int userID0, int userID1){
    float simi = 0.0;
    float sum_V_U = 0.0, sum_U = 0.0, sum_V = 0.0;

    for(int i = 0; i < MOVIES_LENGTH; i++){
        float ratingUserID0 = movies[i].rating[userID0 - 1];
        float ratingUserID1 = movies[i].rating[userID1 - 1];
        if(ratingUserID0 != 0 && ratingUserID1 != 0){
            sum_V_U += ratingUserID0 * ratingUserID1;
            sum_U += ratingUserID0 * ratingUserID0;
            sum_V += ratingUserID1 * ratingUserID1;
        }
    }
    simi = sum_V_U / (sqrt(sum_U) * sqrt(sum_V));

    return simi;
}

void calc_similarity(){
    for(int user0 = 1; user0 < USERS_LENGTH + 1; user0++){
        for(int user1 = 1; user1 < USERS_LENGTH + 1; user1++){
            similarity_all[user0-1][user1-1] = similarity(user0, user1);
        }
    }
}
