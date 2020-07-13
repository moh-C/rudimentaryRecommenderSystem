#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

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
vector<int> topNSimilarities(int, int);
float similarity(int, int);

int main() {
    load_movies();
    load_rating_mat();
    calc_similarity();
    vector<int> topN = topNSimilarities(1, 2);
    cout << topN[0] << " " << topN[1] << endl;



    system("PAUSE");
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
void update_movies_files(vector<movie> movies)
{
    ofstream ofile("Movies.txt");
    for(int i = 0; i < movies.size(); i++)
    {
        ofile<<movies[i].ID<<'\t'<<movies[i].name<<'\t'<<movies[i].year<<'\t';
        ofile<<movies[i].yearDetail<<'\t'<<movies[i].link<<'\t'<<movies[i].genre;
        ofile<<(i == movies.size() - 1 ? '\0' : '\n');
    }
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
            similarity_all[user0 -1][user1-1] = similarity(user0, user1);
        }
    }
}
bool sortbysec(const pair<int,float> &a, const pair<int,float> &b){
    return (a.second < b.second);
}
vector<int> topNSimilarities(int userID, int n){
    vector<pair<int,float>> a;
    vector<int> u;
    for (int i = 0 ; i < USERS_LENGTH; i++){
        a.push_back(make_pair(i + 1, similarity_all[userID-1][i]));
    }
    sort(a.begin(), a.end(), sortbysec);
    for (int i = 0 ; i < n ; i++){
        u.push_back(a[USERS_LENGTH - i - 2].first);
    }
    return u;
}
