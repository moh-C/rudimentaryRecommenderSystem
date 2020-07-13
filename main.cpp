#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <windows.h>
#include <time.h>
#include <bits/stdc++.h>
#include <tgmath.h>
#include <stdlib.h>
#include <conio.h>
#include <tchar.h>

using namespace std;

#define MOVIES_LENGTH 60
#define USERS_LENGTH 50
#define NUMBEROFNEIGHBOURS 1
#define TOPNMOVIES 1

float similarity_all[USERS_LENGTH][USERS_LENGTH];
float similarity_all_duplicate[USERS_LENGTH][USERS_LENGTH];

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

float prediction_value(int, int, vector<int>);
vector<pair<int,float> > predict_nulls(int);
vector<int> prefer_movie(int, int);

void mainPage();
void check(int);


void mainPage(){
    system("CLS");
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN);

    cout << "\n\t\t\tMOVIE RECOMMENDATION SYSTEM\n\n\t\t*********WELCOME TO THE MAIN MENU*********";
    SetConsoleTextAttribute(hStdOut, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
    string s="\n\n\n\t\t1. Display movies\n\t\t2. Display ratings\n\t\t3. Display similarity between two users\n\t\t4. Generate recommendations\n\t\t5. Rate a movie\n\t\t6. Add/Remove a movie\n\t\t7. Exit\n\n\t\tEnter your choice: ";

    for(int i=0;i<s.size();i++){
        cout << s.at(i);
        Sleep(2);
    }
    SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_GREEN);

    int a=0;
    cin >> a;
    check(a);
}


void check(int a)
{
    switch(a)
    {
    case 1:
        system("CLS");
        //Addition();
        break;

    case 2:
        system("CLS");
        //Subtraction();
        break;

    case 3:
        system("CLS");
        //Multiplication();
        break;

    case 4:
        system("CLS");
        //Pheasant_Multiplication();
        break;

    case 5:
        system("CLS");
        //intDivision();
        break;

    case 6:
        system("CLS");
        //LongDivision();
        break;

    case 7:
        system("CLS");
        //Power();
        break;
    default:

        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_RED);

        cout << "\n\t\tThis option is invalid. Try again: ";
        Sleep(2000);
        mainPage();
        break;
    }
}


int main() {
    /*
    load_movies();
    load_rating_mat();
    calc_similarity();
    vector<int> topN = topNSimilarities(1, NUMBEROFNEIGHBOURS);
    */
    mainPage();

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
    vector<pair<int,float> > a;
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

int get_movie_index(int id){
    for(int i = 0; i < movies.size(); i++){
        if(movies[i].ID == id) return i;
    }
    return -1;
}

float prediction_value(int userID, int movieID, vector<int> neighbours){
    float first = 0.0, second = 0.0;
    bool flag = true;

    for(int i=0; i<neighbours.size(); i++)
        if(movies[get_movie_index(movieID)].rating[neighbours[i] - 1] != 0.0){
            flag = false;
        }


    if (flag) return 0;

    for(int i=0; i<neighbours.size(); i++){
        int neighbourID = neighbours[i];
        float similarity = similarity_all[userID - 1][neighbourID - 1];

        first += movies[get_movie_index(movieID)].rating[neighbourID - 1] * similarity;
        second += similarity;
    }
    float prediction_ = first / sqrt(second);
    return prediction_;
}

// Step three of first phase
vector<pair<int,float> > predict_nulls(int userID){
    vector<pair<int,float> > r;
    vector<int> topN = topNSimilarities(userID, NUMBEROFNEIGHBOURS);

    for(int i = 0; i < movies.size() ;i++){
        if(!movies[i].rating[userID - 1]){
            r.push_back(make_pair(i,prediction_value(userID, i, topN)));
        }
    }
    return r;
}

// Start of second Phase
vector<int> prefer_movie(int userID, int topNMovies){
    vector<pair<int,float> > p = predict_nulls(userID);
    vector<int> moviesID;
    if(p.size() == 0)
    {
        return vector<int>();
    }
    int n = topNMovies > p.size() ? p.size() : topNMovies;

    sort(p.begin(), p.end(), sortbysec);
    for (int i = 0 ; i < n; i++){
        moviesID.push_back(p[p.size() - i - 1].first);
    }
    return moviesID;
}
