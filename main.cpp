#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <windows.h>
#include <time.h>
#include <bits/stdc++.h>
#include <conio.h>

using namespace std;

#define MOVIES_LENGTH 60
#define USERS_LENGTH 50
#define NUMBEROFNEIGHBOURS 8
#define TOPNMOVIES 1

// Similarity of users
float similarity_all[USERS_LENGTH][USERS_LENGTH];

// Movies struct
struct movie {
    int ID;
    string name;
    int year;
    string yearDetail;
    string link;
    string genre;
    float rating[50];
};

// Vector of all movies
vector<movie> movies;

// Prototypes for display
void mainPage();
void displayMovies();
void displayRating();
void displaySimilarity();
void generateRecommendation();
void displayRateMovie();
void addRemoveMovie();
void check(char);
void recommendationPrinter(vector<int>, vector<int>, int);
void recommendationPrinterAll(vector<int>, vector<int>, int);
void recommendationPrintOptions();
void addRemovePrintOptions();
void addNewMovieDisplay();
void removeMovieDisplay();
void addRecordByName(string, string);

// Prototypes for display
movie processor(vector<string>);
void load_movies();
void load_rating_mat();
void calc_similarity();
void update_movies_file(vector<movie> movies);
void update_ratings_file(vector<movie> movies);

vector<int> topNSimilarities(int, int);
float similarity(int, int);
float prediction_value(int, int, vector<int>);
vector<pair<int,float> > predict_nulls(int);
vector<int> prefer_movie(int, int);

int get_movie_index(int id);
int get_movieID_by_name(string);
void remove_movie(int);
int string_to_int(string);
string float_to_str(float);
string helper(int, const string&);


// Main function
int main() {
    load_movies();
    load_rating_mat();
    calc_similarity();

    mainPage();

    system("PAUSE");
}

/*-----------------------------------------------------------------------------------------------------------------------------*/
void mainPage(){
    system("CLS");
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN);

    cout << "\n\t\t\tMOVIE RECOMMENDATION SYSTEM\n\n\t\t*********WELCOME TO THE MAIN MENU*********";
    SetConsoleTextAttribute(hStdOut, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
    string s="\n\n\n\t\t1. Display movies\n\t\t2. Display ratings\n\t\t3. Display similarity between two users\n\t\t4. Generate recommendations\n\t\t5. Rate a movie\n\t\t6. Add/Remove a movie\n\t\t7. Exit\n\n\t\tEnter your choice: ";

    for(size_t i=0;i<s.size();i++){
        cout << s.at(i);
        Sleep(2);
    }
    SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_GREEN);

    char character = getch();
    check(character);
    exit(0);
}

void displayMovies() {
    system("CLS");
    fflush(stdout);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "\t-----------------------------\n";
    cout << "\tDisplay Movies\n";
    cout << "\t-----------------------------\n\n";
    cout << "\tMovie id\tMovie name" << setw(50) << " Year\n";
    cout << "\t--------\t----------" << setw(50) << " ----\n";
    for(size_t i=0; i < movies.size(); i++){
        cout << "\t" << movies[i].ID << "\t\t" << left << setw(50) << movies[i].name << "\t" << movies[i].year << "\n";
    }

    cout << "\n\n\t";
    cout << "Press 'm' to Main Menu, and 'q' to Quit\n";

    char c;
    cout << "\t";

    while(1) {
        SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        c = getch();
        if(c == 'm') mainPage();
        else if(c == 'q') exit(0);
        else {
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
            cout << "\n\tThis option is invalid. Try again: ";
        }
    }
}


void displayRating() {
    system("CLS");
    fflush(stdout);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "\t-----------------------------\n";
    cout << "\tDisplay Movies\n";
    cout << "\t-----------------------------\n\n";


    cout << "\tEnter user ID: ";
    string userID = "";
    int userID_int = 0;
    while(1) {
        SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
        getline(cin, userID);
        userID_int = string_to_int(userID);
        if(userID_int <= USERS_LENGTH && userID_int >= 1 ) break;
        else {
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
            cout << "\tThe user id does not exist. Try again: ";
        }
    }

    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "\tMovie id\tRating\n";
    cout << "\t--------\t------\n";
    for(size_t i = 0; i < movies.size(); i++){
        float rating = movies[i].rating[userID_int - 1];
        if(rating)
            cout << "\t" << movies[i].ID << "\t\t" << rating << endl;
    }

    while(1) {
        cout << "\tPress 'r' to retry, 'm' to Main menu, and 'q' to Quit\n";
        char c;
        c = getch();
        if(c == 'm') mainPage();
        else if(c == 'q') exit(0);
        else if(c == 'r') displayRating();
    }
}

void displaySimilarity() {
    system("CLS");
    fflush(stdout);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "\t-----------------------------\n";
    cout << "\tDisplay similarity between two users\n";
    cout << "\t-----------------------------\n\n";

    int userID1_int = 0, userID2_int = 0;
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "\tEnter first user ID: ";
    string userID1 = "";
    while(1) {
        SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
        getline(cin, userID1);
        userID1_int = string_to_int(userID1);
        if(userID1_int <= 50 && userID1_int >= 1 ) break;
        else {
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
            cout << "\tThe user id does not exist. Try again: ";
        }
    }
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "\tEnter second user ID: ";
    string userID2 = "";
    while(1) {
        SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
        getline(cin, userID2);
        userID2_int = string_to_int(userID2);
        if(userID2_int <= 50 && userID2_int >= 1 ) break;
        else {
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
            cout << "\tThe user id does not exist. Try again: ";
        }
    }
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "\tSimilarity between user " << userID1_int << " and " << userID2_int << " is: ";
    cout << similarity_all[userID1_int - 1][userID2_int - 1] << endl << endl;
    while(1) {
        cout << "\tPress 'r' to retry, 'm' to Main menu, and 'q' to Quit\n";
        char c;
        c = getch();
        if(c == 'm') mainPage();
        else if(c == 'q') exit(0);
        else if(c == 'r') displaySimilarity();
    }
}

void generateRecommendation(){
    system("CLS");
    fflush(stdout);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "\t-----------------------------\n";
    cout << "\tGenerate Recommendation\n";
    cout << "\t-----------------------------\n\n";

    int userID_int = 0;
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "\tEnter user ID: ";
    string userID = "";
    while(1) {
        SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
        getline(cin, userID);
        userID_int = string_to_int(userID);
        if(userID_int <= 50 && userID_int >= 1 ) break;
        else {
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
            cout << "\tThe user id does not exist. Try again: ";
        }
    }

    vector<int> nMovies = prefer_movie(userID_int, 60);
    vector<int> neighbours = topNSimilarities(userID_int, NUMBEROFNEIGHBOURS);

    // Top three recommendations
    recommendationPrinter(nMovies, neighbours, userID_int);

    recommendationPrintOptions();

    string choice = "";
    while(1) {
        SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
        getline(cin, choice);
        if(choice == "a"){
            recommendationPrinterAll(nMovies, neighbours, userID_int);
            recommendationPrintOptions();
        } else if(choice == "b"){
            generateRecommendation();
        } else if(choice == "c"){
            mainPage();
        } else if(choice == "d"){
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            exit(0);
        } else {
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
            cout << "\tInvalid option. Please try again: ";
        }
    }
}

void displayRateMovie() {
    system("CLS");
    fflush(stdout);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "\t-----------------------------\n";
    cout << "\tRate a movie\n";
    cout << "\t-----------------------------\n";

    cout << "\n\n\tEnter the Information Below:\n";
    string userID = "",movieID = "", rating = "", choice = "";
    int userID_int = 0, movieID_int = 0, rating_int = 0;
    cout << "\t=============================\n";

    cout << "\tUser ID: ";
    SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
    getline(cin, userID);
    userID_int = string_to_int(userID);
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "\tMovie ID: ";
    SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
    getline(cin, movieID);
    movieID_int = string_to_int(movieID);
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "\tRating (1-5): ";
    SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
    getline(cin, rating);
    rating_int = string_to_int(rating);
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    cout << "\t=============================\n";
    cout << "\tDo you want to save (Y/N)? ";
    getline(cin, choice);

    if(choice == "y" || choice == "Y"){
        if(userID_int > USERS_LENGTH || userID_int < 1 ){
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
            cout << "\n\tThis user id does not exist.\n\n\n";
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
        else if(get_movie_index(movieID_int) == -1){
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
            cout << "\n\tThis movie id does not exist.\n\n\n";
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
        else if(rating_int > 5 || rating_int < 1){
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
            cout << "\n\tThis rating is not valid. (only between 1 to 5)\n\n\n";
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
        else {
            movies[get_movie_index(movieID_int)].rating[userID_int-1] = rating_int;
            update_ratings_file(movies);
            cout<< "\n\tThe record was successfully saved\n\n\n";
        }
    }
    while(1) {
        cout << "\tPress 'r' to retry, 'm' to Main menu, and 'q' to Quit\n\t";
        char c;
        c = getch();
        if(c == 'm') mainPage();
        else if(c == 'r') displayRateMovie();
        else if(c == 'q') exit(0);
    }
}

void addRemoveMovie(){
    system("CLS");
    fflush(stdout);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "\t-----------------------------\n";
    cout << "\tAdd/Remove a movie\n";
    cout << "\t-----------------------------\n";

    addRemovePrintOptions();

    string choice = "";
    while(1) {
        SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
        getline(cin, choice);
        if(choice == "a"){
            // Add a new movie
            addNewMovieDisplay();
        } else if(choice == "b"){
            // Remove a movie
            removeMovieDisplay();
        } else if(choice == "c"){
            mainPage();
        } else if(choice == "d"){
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            exit(0);
        } else {
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
            cout << "\tInvalid option. Please try again: ";
        }
    }
}

void addNewMovieDisplay(){
    while(1) {
        system("CLS");
        fflush(stdout);
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        cout << "\t-----------------------------\n";
        cout << "\tAdd a new movie\n";
        cout << "\t-----------------------------\n";
        cout << "\n\tEnter the information below\n";
        cout << "\t=============================\n";

        string name = "", choice = "", year = "";
        cout << "\n\tName: ";
        SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
        getline(cin, name);
        SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        cout << "\n\tYear: ";
        SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
        getline(cin, year);
        SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        string save = "";
        cout << "\tDo you want to save (Y/N)? ";
        SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
        getline(cin, save);

        if(save == "y" || save == "Y"){
            if(get_movieID_by_name(name) != -1) {
                SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
                cout<<"\tThe record already exists.\n";
                SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            else {
                SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
                addRecordByName(name, year);
                cout<<"\tThe record was saved successfully.\n";
                SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
        }

        cout << "\n\tPress 'r' to retry,'p' to Previous, 'm' to Main menu, and 'q' to Quit\n\t";
        char c;
        c = getch();
        if(c == 'm') mainPage();
        else if(c == 'q') exit(0);
        else if(c == 'r') addNewMovieDisplay();
        else if(c == 'p') addRemoveMovie();
    }
}

void removeMovieDisplay(){
    while(1) {
        system("CLS");
        fflush(stdout);
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        cout << "\t-----------------------------\n";
        cout << "\tRemove a new movie\n\n";
        cout << "\t-----------------------------\n";
        cout << "\t====Remove by name or ID?====\n";
        cout << "\ta. By name\n";
        cout << "\tb. By ID\n";
        cout << "\tYour choice: ";
        SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
        char character = getch();
        SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        if(character == 'a'){
            string movie_name = "",choice = "";
            cout<<"\n\tEnter the name of movie: ";
            SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
            getline(cin, movie_name);
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            cout<<"\n\tAre you sure you want to delete \""<<movie_name<<"\"(Y/N) ?";
            SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
            getline(cin, choice);
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            if(get_movieID_by_name(movie_name) != -1)
            {
                cout<<"\tThe record was successfully removed.\n";
                remove_movie(get_movieID_by_name(movie_name));
            }
            else
            {
                SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
                cout<<"\tThe movie does not exist.\n";

                SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
        }
        else if(character == 'b'){
            string movieID = "",choice = "";
            int movieID_int = 0;
            cout<<"\n\tEnter movie ID: ";
            SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
            getline(cin, movieID);
            movieID_int = string_to_int(movieID);
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            cout<<"\n\tAre you sure you want to delete \"movie "<<movieID_int<<"\"(Y/N) ?";
            SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
            getline(cin, choice);
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            if(get_movie_index(movieID_int) != -1)
            {
                cout<<"\tThe record was successfully removed.\n";
                remove_movie(movieID_int);
            }
            else
            {
                SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
                cout<<"\tThe movie does not exist.\n\n";

                SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
        }
        else{
            continue;
        }
        cout << "\tPress 'r' to retry,'p' to Previous, 'm' to Main menu, and 'q' to Quit\n";
        char c;
        c = getch();
        if(c == 'm') mainPage();
        else if(c == 'q') exit(0);
        else if(c == 'r') removeMovieDisplay();
        else if(c == 'p') addRemoveMovie();
    }
}

void check(char a) {
    switch(a)
    {
    case '1':
        system("CLS");
        displayMovies();
        break;

    case '2':
        system("CLS");
        displayRating();
        break;

    case '3':
        system("CLS");
        displaySimilarity();
        break;

    case '4':
        system("CLS");
        generateRecommendation();
        break;

    case '5':
        system("CLS");
        displayRateMovie();
        break;

    case '6':
        system("CLS");
        addRemoveMovie();
        break;

    case '7':
        exit(0);
        break;
    default:

        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

        cout << "\n\t\tThis option is invalid. Try again: ";
        char character = getch();
        check(character);
        break;
    }
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
void recommendationPrinter(vector<int> nMovies, vector<int> neighbours, int userID_int){
    int cnt = 3;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    printf("\n\tTop-%d Recommendations for user %d are:\n\n", cnt, userID_int);

    cout << left << setw(15) << "\tMovie ID" << left << setw(50) << "Movie name" << left << setw(10) << "Year" << left << setw(15) << "Predicted Rating\n";
    cout << left << setw(15) << "\t--------" << left << setw(50) << "----------" << left << setw(10) << "----" << left << setw(15) << "----------------\n\t";

    for(int i = 0; i < cnt; i++){
        int movieID = nMovies[i];
        float value = prediction_value(userID_int, movieID, neighbours);
        float nearest = roundf(value * 100) / 100;

        cout << left << setw(15) << movies[movieID].ID;
        cout << left << setw(50) << movies[movieID].name;
        cout << left << setw(10) << movies[movieID].year;
        cout << left << helper(15, float_to_str(nearest));
        cout << "\n\t";
    }
}

void recommendationPrinterAll(vector<int> nMovies, vector<int> neighbours, int userID_int){
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, 0x09);
    printf("\n\n\tPrediction of unseen movies by user %d: \n\n", userID_int);
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    cout << left << setw(15) << "\tMovie ID" << left << setw(15) << "Predicted Rating\n";
    cout << left << setw(15) << "\t--------" << left << setw(15) << "----------------\n\t";

    for(size_t i = 0; i < nMovies.size(); i++){
        int movieID = nMovies[i];
        float value = prediction_value(userID_int, movieID, neighbours);
        float nearest = roundf(value * 100) / 100;

        cout << left << setw(15) << movies[movieID].ID;
        if(nearest == 0.0f) {
            cout << left << "Not predictable";
        }
        else cout << left << helper(15, float_to_str(nearest));
        cout << "\n\t";
    }
}

void recommendationPrintOptions(){
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, 0x09);
    cout << "\n\n\t====What do you want to do?====\n";
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    cout << "\ta. Display all predictions for this user" << endl;
    cout << "\tb. Retry" << endl;
    cout << "\tc. Back to main menu" << endl;
    cout << "\td. Exit" << endl;
    cout << "\n\tYour Choice: ";
}

void addRemovePrintOptions(){
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, 0x09);
    cout << "\n\n\t====What do you want to do?====\n";
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    cout << "\ta. Add a new movie" << endl;
    cout << "\tb. Remove a movie" << endl;
    cout << "\tc. Back to main menu" << endl;
    cout << "\td. Exit" << endl;
    cout << "\n\tYour Choice: ";
}

int get_movieID_by_name(string movie_name) {
    for(size_t i = 0; i < movies.size(); i++)
        if(movies[i].name == movie_name)
            return movies[i].ID;
    return -1;
}

void addRecordByName(string name, string year){
    movie newMovie;
    newMovie.genre = "null";
    newMovie.link = "null";
    newMovie.year = string_to_int(year);
    newMovie.name = name;
    newMovie.ID = movies[movies.size() - 1].ID + 1;
    for(int i=0; i < 50 ;i++)
        newMovie.rating[i] = 0.0f;
    newMovie.yearDetail = "null";

    movies.push_back(newMovie);
    update_movies_file(movies);
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

    for(int i=0; i < MOVIES_LENGTH; i++){
        for(int j=0; j < USERS_LENGTH; j++){
            movies[i].rating[j] = 0;
        }
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

void update_movies_file(vector<movie> movies) {
    ofstream ofile("Movies.txt");
    for(size_t  i = 0; i < movies.size(); i++) {
        ofile<<movies[i].ID<<'\t'<<movies[i].name<<'\t'<<movies[i].year<<'\t';
        ofile<<movies[i].yearDetail<<'\t'<<movies[i].link<<'\t'<<movies[i].genre;
        if(i != movies.size() - 1){
            ofile<<endl;
        }
    }
}

void update_ratings_file(vector<movie> movies){
    ofstream ofile("Ratings.txt");
    for(int i = 0; i < USERS_LENGTH; i++) {
        for(size_t j = 0; j < movies.size(); j++) {
            if(static_cast<int>(movies[j].rating[i]) != 0){
                ofile<<(i+1)<<'\t'<<movies[j].ID<<'\t'<<static_cast<int>(movies[j].rating[i]);
                if(j != movies.size() - 1 || i != USERS_LENGTH - 1)
                    ofile<<endl;
            }
        }
    }
}

void remove_movie(int movieID) {
    int movie_index = get_movie_index(movieID);
    movies.erase(movies.begin() + movie_index);
    update_movies_file(movies);
    update_ratings_file(movies);
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
    for(size_t i = 0; i < movies.size(); i++){
        if(movies[i].ID == id) return i;
    }
    return -1;
}

float prediction_value(int userID, int movie_index, vector<int> neighbours){
    float first = 0.0, second = 0.0;
    bool flag = true;

    for(size_t i=0; i<neighbours.size(); i++)
        if(movies[movie_index].rating[neighbours[i] - 1] != 0.0){
            flag = false;
        }

    if (flag) return 0;

    for(size_t i=0; i<neighbours.size(); i++){
        int neighbourID = neighbours[i];
        float similarity = similarity_all[userID - 1][neighbourID - 1];

        first += movies[movie_index].rating[neighbourID - 1] * similarity;
        second += similarity;
    }
    float prediction_ = first / second;
    return prediction_;
}

// Step three of first phase
vector<pair<int,float> > predict_nulls(int userID){
    vector<pair<int,float> > r;
    vector<int> topN = topNSimilarities(userID, NUMBEROFNEIGHBOURS);

    for(size_t i = 0; i < movies.size() ;i++){
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
    if(p.size() == 0) {
        return vector<int>();
    }
    size_t n = (size_t)topNMovies > p.size() ? p.size() : topNMovies;

    sort(p.begin(), p.end(), sortbysec);
    for (size_t i = 0 ; i < n; i++){
        moviesID.push_back(p[p.size() - i - 1].first);
    }
    return moviesID;
}

/*--------------------------------------------------------------------------------------------------------------------------*/

string helper(int width, const string& str) {
    int len = str.length();
    if(width < len) { return str; }

    int diff = width - len;
    int pad1 = diff/2;
    int pad2 = diff - pad1;
    return string(pad1, ' ') + str + string(pad2, ' ');
}

int string_to_int(string str) {
    int res = 0;

    string s = str;
    stringstream stoi(s);

    stoi >> res;
    return res;
}

string float_to_str(float val){
    stringstream stream;
    stream << val;
    string res = stream.str();
    return res;
}
