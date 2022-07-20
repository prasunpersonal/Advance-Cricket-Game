// ========================================================
// Project Name:   Advance Cricket Game in C++
// Project For:    Practice
// Author:         Prasun Sarkar
// ========================================================

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<iomanip>
#include<ctime>
#include<unistd.h>
#include<conio.h>
using namespace std;

vector<string> allPlayers = {"R Sharma", "H Pandya", "B Kumar", "S Dhawan", "R Jadeja", "J Bumrah", "V Kohli", "K Pandya", "C Sakariya", "KL Rahul", "A Patel", "Y Chahal", "P Shaw", "S Raina", "D Chahar", "R Pant", "K Jadav", "S Thakur", "S Iyre", "S Dube", "V Chakravarti", "MS Dhoni", "W Sundar", "H Patel", "S Samson", "D Hooda", "R Ashwin", "M Pandey", "R Tewatia", "M Siraj"};
vector<string> runs = {"Dot ball","1 run","2 runs","3 runs","4 runs","5 runs","6 runs","OUT"};
vector<int> battingSequence, bowlingSequence;
string toss_win, toss_loose, current_batting, current_bowling, lastover_bowled;
int totalOvers, totalPlayers, battingIndex1, battingIndex2, bowlingIndex;
bool firstInnings = true;

class Player{
    public:
        string playerName;
        int runsScored = 0, ballPlayed = 0, wickets = 0, runsGiven = 0;
        bool out = false, halfCentury = false, century = false;
        vector<string> bowlingSpell;

        Player(string s){
            playerName = s;
        }
};

class Team{
    public:
        string teamName;
        vector<Player> players;
        bool battingStarted = false;

        void newPlayer(string s){
            Player p(s);
            this->players.push_back(p);
        }
        int teamRuns(){
            int x=0;
            for(int i=0; i<this->players.size(); i++){ 
                x += this->players.at(i).runsScored; 
            }
            return x;
        }
        int pitchedBalls(){
            int x=0;
            for(int i=0; i<this->players.size(); i++){ 
                x += this->players.at(i).bowlingSpell.size(); 
            }
            return x;
        }
        int teamWickets(){
            int x=0;
            for(int i=0; i<this->players.size(); i++){
                if(this->players.at(i).out){
                    x++;
                }
            }
            return x;
        }
        void printPlayers(){
            cout<<"\t>> Team list of "<<this->teamName<<endl;
            cout<<"\t==========================================================="<<endl;
            cout<<"\t|"<<left<<setw(31)<<"\tPlayer Name"<<left<<setw(20)<<"Player Type"<<"|"<<endl;
            cout<<"\t|---------------------------------------------------------|"<<endl;
            for(int i=0; i<totalPlayers; i++){
                string playerType;
                int index = find(allPlayers.begin(), allPlayers.end(), this->players.at(i).playerName) - allPlayers.begin();
                if(index%3 == 0){
                    playerType = "Batsman";
                }else if(index%3 == 1){
                    playerType = "All Rounder";
                }else if(index%3 == 2){
                    playerType = "Bowler";
                }
                cout<<"\t|"<<left<<setw(31)<<"\t["+to_string(i+1)+"] -> "+this->players.at(i).playerName<<left<<setw(20)<<playerType<<"|"<<endl;
            }
            cout<<"\t==========================================================="<<endl<<endl;
        }
};

void startingPage(){
    cout<<"\t----------------------------------------------------------------------------------------"<<endl
        <<"\t|============================| WELCOME TO VIRTUAL CRICKET |============================|"<<endl
        <<"\t|--------------------------------------------------------------------------------------|"<<endl
        <<"\t|==========================| Copyright (C) Prasun Sarkar... |==========================|"<<endl
        <<"\t----------------------------------------------------------------------------------------"<<endl<<endl;

    cout<<"\t----------------------------------------------------------------------------------------"<<endl
        <<"\t|===================================| Instructions |===================================|"<<endl
        <<"\t|--------------------------------------------------------------------------------------|"<<endl
        <<"\t| 1. You can select the number of players in each team (Maximum 11 and Minimum 4).     |"<<endl
        <<"\t| 2. You can select number of overs will be in each innings.                           |"<<endl
        <<"\t| 3. Create two teams from the given pool of 30 players.                               |"<<endl
        <<"\t| 4. You can't select a player who is already selected for any team.                   |"<<endl
        <<"\t| 5. Lead the toss and decide the choice of play.                                      |"<<endl
        <<"\t| 6. In this game you can select each batsman and bowler.                              |"<<endl
        <<"\t| 7. You can't select a batsman who has already been out.                              |"<<endl
        <<"\t| 8. You can't select a bowler who has bowled the last over or completed his spell.    |"<<endl
        <<"\t----------------------------------------------------------------------------------------"<<endl<<endl;
    
    cout<<"\tPress Enter to start the game >> ";
    while(getch() != 13);
    cout<<endl<<endl;
}

void gameInitialisation(){
    player_number_lebel:
    cout<<"\tHow many players will be in each team: ";
    while (!(cin>>totalPlayers)) {
		cout<<"\tPlease enter a number."<<endl<<"\tHow many players will be in each team: ";
		cin.clear();
		cin.ignore(256, '\n');
	}
    if(totalPlayers > 11){
        cout<<"\tA team can have maximum 11 players."<<endl<<endl;
        goto player_number_lebel;
    }else if(totalPlayers < 4){
        cout<<"\tA team must have at least 4 players."<<endl<<endl;
        goto player_number_lebel;
    }
    cout<<"\tHow many overs you want to play: ";
    while (!(cin>>totalOvers)) {
		cout<<"\tPlease enter a number."<<endl<<"\tHow many overs you want to play: ";
		cin.clear();
		cin.ignore(256, '\n');
	}
    cout<<endl;
}

string ballToOver(int balls){
    string overs;
    if(balls % 6 == 0){
        overs = to_string(balls / 6);
    }else{
        overs = to_string(balls / 6) + "." + to_string(balls % 6);
    }
    return overs;
}

void celebration(Team &batting, Team &bowling){
    if(batting.players.at(battingIndex1).runsScored >= 50 && !batting.players.at(battingIndex1).halfCentury){
        cout<<"\tHalf Century for "<<batting.players.at(battingIndex1).playerName<<"!\n\tCongratulations to "<<batting.players.at(battingIndex1).playerName<<" for this Half Century!\n\tA big hand for his excelent batting performance!"<<endl<<endl;
        batting.players.at(battingIndex1).halfCentury = true;
    }
    if(batting.players.at(battingIndex1).runsScored >= 100 && !batting.players.at(battingIndex1).century){
        cout<<"\tHalf Century for "<<batting.players.at(battingIndex1).playerName<<"!\n\tCongratulations to "<<batting.players.at(battingIndex1).playerName<<" for this Century!\n\tA big hand for his outstanding batting performance!"<<endl<<endl;
        batting.players.at(battingIndex1).century = true;
    }
    if(bowling.players.at(bowlingIndex).bowlingSpell.back() == "W"){
        int count=0, l = bowling.players.at(bowlingIndex).bowlingSpell.size();
        for(int i=0; i<l; i++){
            if(bowling.players.at(bowlingIndex).bowlingSpell.at(l - i - 1) == "W"){
                count++;
            }else{
                break;
            }
        }
        if(count == 3){
            cout<<"\tHat-Trick for "<<bowling.players.at(bowlingIndex).playerName<<"!\n\tCongratulations to "<<bowling.players.at(bowlingIndex).playerName<<" for this Hat-Trick!\n\tA big hand for his excelent bowling performance!"<<endl<<endl;
        }
    }

}

int playerRun(){
    srand(time(NULL));
    return rand() % 8;
}

bool playerAvailable(Team t1, Team t2, string s){
    for (int i = 0; i < t1.players.size(); i++){
        if (t1.players.at(i).playerName == s){
            return false;
        }
    }
    for (int i = 0; i < t2.players.size(); i++){
        if (t2.players.at(i).playerName == s){
            return false;
        }
    }
    return true;
}

void showPlayerList(){
    int index = 0;
    cout<<"\t"<<left<<setw(30)<<"Batsmen"<<left<<setw(30)<<"All Rounders"<<left<<setw(30)<<"Bowlers"<<endl;
    for(int i=0; i<10; i++){
        cout<<"\t";
        for(int j=0; j<3; j++){
            string s;
            if(index+1 < 10){
                s = "[0" + to_string(index+1) + "] => " + allPlayers.at(index);
            }else{
                s = "[" + to_string(index+1) + "] => " + allPlayers.at(index);
            }
            cout<<left<<setw(30)<<s;
            index++;
        }
        cout<<endl;
    }
    cout<<endl;
}

void teamSelection(Team &t1, Team &t2){
    cout<<"\tPress Enter to select your teams >> ";
    while(getch() != 13);
    cout<<endl<<endl;
    showPlayerList();
    for(int i=0; i<totalPlayers; i++){
        int select;
        select_lebel_1:
        cout<<"\tSelect Player-"<<i+1<<" for "<<t1.teamName<<": ";
        while (!(cin>>select)) {
            cout<<"\tPlease enter proper index."<<endl<<"\tSelect Player-"<<i+1<<" for "<<t1.teamName<<": ";
            cin.clear();
            cin.ignore(256, 13);
	    }
        if(select >= 1 && select <= allPlayers.size()){
            if(playerAvailable(t1,t2,allPlayers.at(select-1))){
                t1.newPlayer(allPlayers.at(select-1));
            }else{
                cout<<"\tThe player is already selected. Please select another player."<<endl;
                goto select_lebel_1;
            }
        }else{
            cout<<"\tPlease select from the given pool."<<endl;
            goto select_lebel_1;
        }

        select_lebel_2:
        cout<<"\tSelect Player-"<<i+1<<" for "<<t2.teamName<<": ";
        while (!(cin>>select)) {
            cout<<"\tPlease enter proper index."<<endl<<"\tSelect Player-"<<i+1<<" for "<<t2.teamName<<": ";
            cin.clear();
            cin.ignore(256, '\n');
	    }
        if(select <= allPlayers.size()){
            if(playerAvailable(t1,t2,allPlayers.at(select-1))){
                t2.newPlayer(allPlayers.at(select-1));
            }else{
                cout<<"\tThe player is already selected. Please select another player."<<endl;
                goto select_lebel_2;
            }
        }else{
            cout<<"\tPlease select from the given pool."<<endl;
            goto select_lebel_2;
        }
    }
    cout<<endl;
    t1.printPlayers();
    t2.printPlayers();
}

void toss(Team t1, Team t2){
    int x;
    srand(time(0));
    int coin = rand()%2;
    cout<<"\tPress Enter to toss the coin >> ";
    while(getch() != 13);
    cout<<endl<<endl<<"\tTossing the coin..."<<endl<<"\tPlease wait...";
    sleep(1);
    cout<<endl<<endl;
    if(coin == 0){
        toss_win = t1.teamName;
        toss_loose = t2.teamName;
    }else{
        toss_win = t2.teamName;
        toss_loose = t1.teamName;
    }
    cout<<"\t"<<toss_win<<" won the toss."<<endl<<endl;
    toss_lebel:
    cout<<"\tEnter 1 to Batting or 2 to Bowling first: ";
    while (!(cin>>x)) {
            cout<<"\tPlease enter a valid choice..."<<endl<<"\tEnter 1 to Batting or 2 to Bowling first: ";
            cin.clear();
            cin.ignore(256, 13);
        }
    if(x == 1){
        cout<<endl<<"\t"<<toss_win<<" won the toss and elected to bat first."<<endl<<endl;
        current_batting = toss_win;
        current_bowling = toss_loose;
    }else if(x == 2){
        cout<<endl<<"\t"<<toss_win<<" won the toss and elected to bowl first."<<endl<<endl;
        current_batting = toss_loose;
        current_bowling = toss_win;
    }else{
        cout<<"\tPlease enter a valid choice..."<<endl;
        goto toss_lebel;
    }
}

void newBatsman(Team t){
    if(!t.battingStarted){
        newBatsman_lebel0:
        cout<<"\tPlease select Batsman-1 of "<<t.teamName<<": ";
        while (!(cin>>battingIndex1)) {
            cout<<"\tPlease enter proper index."<<endl<<"\tPlease select Batsman-1 of "<<t.teamName<<": ";
            cin.clear();
            cin.ignore(256, '\n');
        }
        if(battingIndex1<1 || battingIndex1>totalPlayers){
            cout<<"\tPlease enter a proper index."<<endl;
            goto newBatsman_lebel0;
        }
        battingIndex1--;
        battingSequence.push_back(battingIndex1);

        newBatsman_lebel1:
        cout<<"\tPlease select Batsman-2 of "<<t.teamName<<": ";
        while (!(cin>>battingIndex2)) {
            cout<<"\tPlease enter proper index."<<endl<<"\tPlease select Batsman-2 of "<<t.teamName<<": ";
            cin.clear();
            cin.ignore(256, '\n');
        }
        if(battingIndex2<1 || battingIndex2>totalPlayers){
            cout<<"\tPlease enter a proper index."<<endl;
            goto newBatsman_lebel1;
        }
        battingIndex2--;
        if(battingIndex2 == battingIndex1){
            cout<<"\tThis player is already selected as Batsman-1."<<endl;
            goto newBatsman_lebel1;
        }
        battingSequence.push_back(battingIndex2);
    }else{
        int x;
        newBatsman_lebel2:
        cout<<"\tPlease select Batsman-"<<t.teamWickets()+2<<" of "<<t.teamName<<": ";
        while (!(cin>>x)) {
            cout<<"\tPlease enter proper index."<<endl<<"\tPlease select Batsman-"<<t.teamWickets()+2<<" of "<<t.teamName<<": ";
            cin.clear();
            cin.ignore(256, '\n');
        }
        if(x<1 || x>totalPlayers){
            cout<<"\tPlease enter a proper index."<<endl;
            goto newBatsman_lebel2;
        }
        x--;
        if(t.players.at(x).out){
            cout<<"\tYou can't select a player who has already been OUT!"<<endl;
            goto newBatsman_lebel2;
        }else if(x==battingIndex2){
            cout<<"\tThis player is already batting."<<endl;
            goto newBatsman_lebel2;
        }else{
            battingIndex1 = x;
            battingSequence.push_back(x);
        }
    }
    cout<<endl;
}

void newBowler(Team t){
    int x;
    int maxOver;
    if(totalOvers % (totalPlayers / 2) == 0){
        maxOver = totalOvers / (totalPlayers / 2);
    }else{
        maxOver = (totalOvers / (totalPlayers / 2)) + 1;
    }
    newBowler_lebel:
    cout<<"\tPlease select a Bowler of "<<t.teamName<<": ";
    while (!(cin>>x)) {
        cout<<"\tPlease enter proper index."<<endl<<"\tPlease select a Bowler of "<<t.teamName<<": ";
        cin.clear();
        cin.ignore(256, '\n');
    }
    if(x<1 || x>totalPlayers){
        cout<<"\tPlease enter a proper index."<<endl;
        goto newBowler_lebel;
    }
    x--;
    if(t.players.at(x).bowlingSpell.size() / 6 < maxOver){
        if(t.players.at(x).playerName == lastover_bowled){
            cout<<"\tSame bowler can't continue the next over!"<<endl;
            goto newBowler_lebel;
        }else{
            bowlingIndex = x;
            if(find(bowlingSequence.begin(), bowlingSequence.end(), x) == bowlingSequence.end()){
                bowlingSequence.push_back(x);
            }
        }
    }else{
        cout<<"\tA bowler can bowl maximum "<<maxOver<<" over(s) in a "<<totalOvers<<" over(s) match!"<<endl;
        goto newBowler_lebel;
    }
    cout<<endl;
}

void printInningsStart(){
    if(firstInnings){
        cout<<"\tPress Enter to start first innings >> ";
        while(getch() != 13);
        cout<<endl<<endl;

        cout<<"\t----------------------------------------------------------------------------------------"<<endl
            <<"\t|===============================| FIRST INNINGS STARTS |===============================|"<<endl
            <<"\t----------------------------------------------------------------------------------------"<<endl<<endl;
    }else{
            cout<<"\tPress Enter to start second innings >> ";
            while(getch() != 13);
            cout<<endl<<endl;

        cout<<"\t----------------------------------------------------------------------------------------"<<endl
            <<"\t|============================| STARTING OF SECOND INNINGS |============================|"<<endl
            <<"\t----------------------------------------------------------------------------------------"<<endl<<endl;
    }
}

void inningsResult(Team t1, Team t2){
    string s1, s2;
    int l = (battingSequence.size() > bowlingSequence.size()) ? battingSequence.size() : bowlingSequence.size();
    cout<<"\t----------------------------------------------------------------------------------------"<<endl
        <<"\t|==================================| INNINGS RESULT |==================================|"<<endl
        <<"\t|--------------------------------------------------------------------------------------|"<<endl;
    for(int i=0; i<l; i++){
        if(i < battingSequence.size()){
            s1 = t1.players.at(battingSequence.at(i)).playerName + " - " + to_string(t1.players.at(battingSequence.at(i)).runsScored) + " (" + to_string(t1.players.at(battingSequence.at(i)).ballPlayed) + ")";
        }else{
            s1 = "";
        }
        if(i < bowlingSequence.size()){
            s2 = t2.players.at(bowlingSequence.at(i)).playerName + " - " + to_string(t2.players.at(bowlingSequence.at(i)).runsGiven) + "-" + to_string(t2.players.at(bowlingSequence.at(i)).wickets) + " (" + ballToOver(t2.players.at(bowlingSequence.at(i)).bowlingSpell.size()) + ")";
        }else{
            s2 = "";
        }
        cout<<"\t|\t"<<left<<setw(50)<<s1<<left<<setw(29)<<s2<<"|"<<endl;
    }
    cout<<"\t----------------------------------------------------------------------------------------"<<endl<<endl;
    if(firstInnings){
        cout<<"\t"<<t2.teamName<<" needs "<<t1.teamRuns()+1<<" runs to win this match."<<endl<<endl;
    }
}

void printInningsEnd(){
    if(firstInnings){
        cout<<"\t----------------------------------------------------------------------------------------"<<endl
            <<"\t|===============================| END OF FIRST INNINGS |===============================|"<<endl
            <<"\t----------------------------------------------------------------------------------------"<<endl<<endl;
    }else{
        cout<<"\t----------------------------------------------------------------------------------------"<<endl
            <<"\t|===================================| END OF MATCH |===================================|"<<endl
            <<"\t----------------------------------------------------------------------------------------"<<endl<<endl;
    }
}

void printPerBall(Team batting, Team bowling, vector<string> thisOver){
    string score = batting.teamName + ": " + to_string(batting.teamRuns()) + "-" + to_string(batting.teamWickets());
    string overs = "Overs: " + ballToOver(bowling.pitchedBalls());
    string batsman1 = batting.players.at(battingIndex1).playerName + ": " + to_string(batting.players.at(battingIndex1).runsScored) + " (" + to_string(batting.players.at(battingIndex1).ballPlayed) + ")";
    string batsman2 = batting.players.at(battingIndex2).playerName + ": " + to_string(batting.players.at(battingIndex2).runsScored) + " (" + to_string(batting.players.at(battingIndex2).ballPlayed) + ")";
    string bowler = bowling.players.at(bowlingIndex).playerName + ": " + to_string(bowling.players.at(bowlingIndex).runsGiven) + "-" + to_string(bowling.players.at(bowlingIndex).wickets) + " (" + ballToOver(bowling.players.at(bowlingIndex).bowlingSpell.size()) + ")";
    string overRuns = "This Over: ";
    for(int i=0; i<thisOver.size(); i++){
        overRuns += thisOver.at(i) + " ";
    }
    cout<<"\t";
    for(int i=0; i<88; i++){cout<<"=";}
    cout<<endl;
    cout<<"\t\t"<<left<<setw(20)<<score<<left<<setw(30)<<batsman1<<bowler<<endl;
    cout<<"\t\t"<<left<<setw(20)<<overs<<left<<setw(30)<<batsman2<<overRuns<<endl;
    cout<<"\t";
    for(int i=0; i<88; i++){cout<<"=";}
    cout<<endl<<endl;
}

void matchSummary(Team t1, Team t2){
    cout<<"\t========================================================================================"<<endl
        <<"\t|-------------------------------| SUMMARY OF THE MATCH |-------------------------------|"<<endl;
    for(int i=0; i<2; i++){
        Team batting, bowling;
        if(i==0){
            batting = t1; 
            bowling = t2;
        }else{
            batting = t2; 
            bowling = t1;
        }
        
        string s = "| " + batting.teamName + ": " + to_string(batting.teamRuns()) + "-" + to_string(batting.teamWickets()) + " (" + ballToOver(bowling.pitchedBalls()) + ") |";

        for(int j=0; j<batting.players.size(); j++){
            if(batting.players.at(j).runsScored == 0){
                batting.players.erase(batting.players.begin() + j);
                j--;
            }
        }
        
        for(int j=0; j<bowling.players.size(); j++){
            if(bowling.players.at(j).bowlingSpell.size() == 0){
                bowling.players.erase(bowling.players.begin() + j);
                j--;
            }
        }

        for(int j=0; j<batting.players.size()-1; j++){
            for(int k=j+1; k<batting.players.size(); k++){
                if(batting.players.at(j).runsScored < batting.players.at(k).runsScored){
                    Player tmp = batting.players.at(j);
                    batting.players.at(j) = batting.players.at(k);
                    batting.players.at(k) = tmp;
                }else if(batting.players.at(j).runsScored == batting.players.at(k).runsScored){
                    if(batting.players.at(j).ballPlayed > batting.players.at(k).ballPlayed){
                        Player tmp = batting.players.at(j);
                        batting.players.at(j) = batting.players.at(k);
                        batting.players.at(k) = tmp;
                    }else if(batting.players.at(j).ballPlayed == batting.players.at(k).ballPlayed){
                        if(batting.players.at(j).out && !batting.players.at(k).out){
                            Player tmp = batting.players.at(j);
                            batting.players.at(j) = batting.players.at(k);
                            batting.players.at(k) = tmp;
                        }
                    }
                }
            }
        }
        for(int j=0; j<bowling.players.size()-1; j++){
            for(int k=j+1; k<bowling.players.size(); k++){
                if(bowling.players.at(j).wickets < bowling.players.at(k).wickets){
                    Player tmp = bowling.players.at(j);
                    bowling.players.at(j) = bowling.players.at(k);
                    bowling.players.at(k) = tmp;
                }else if(bowling.players.at(j).wickets == bowling.players.at(k).wickets){
                    if(bowling.players.at(j).runsGiven > bowling.players.at(k).runsGiven){
                        Player tmp = bowling.players.at(j);
                        bowling.players.at(j) = bowling.players.at(k);
                        bowling.players.at(k) = tmp;
                    }else if(bowling.players.at(j).runsGiven == bowling.players.at(k).runsGiven){
                        if(bowling.players.at(j).bowlingSpell.size() > bowling.players.at(k).bowlingSpell.size()){
                            Player tmp = bowling.players.at(j);
                            bowling.players.at(j) = bowling.players.at(k);
                            bowling.players.at(k) = tmp;
                        }
                    }
                }
            }
        }
        string sfill;
        for(int j=0; j<87-s.size(); j++){sfill.push_back('+');}
        sfill.push_back('|');
        cout<<"\t|======================================================================================|"<<endl
            <<"\t"<<s<<sfill<<endl
            <<"\t|======================================================================================|"<<endl;
        for(int j=0; j<3; j++){
            string s1, s2;
            if(j < batting.players.size()){
                s1 = batting.players.at(j).playerName + " - " + to_string(batting.players.at(j).runsScored) + " (" + to_string(batting.players.at(j).ballPlayed) + ")";
            }else{
                s1 = "";
            }
            if(j < bowling.players.size()){
                s2 = bowling.players.at(j).playerName + " - " + to_string(bowling.players.at(j).runsGiven) + "-" + to_string(bowling.players.at(j).wickets) + " (" + ballToOver(bowling.players.at(j).bowlingSpell.size()) + ")";
            }else{
                s2 = "";
            }
            cout<<"\t|\t"<<left<<setw(50)<<s1<<left<<setw(29)<<s2<<"|"<<endl;
        }
    }
    cout<<"\t========================================================================================"<<endl;
}

void mainGame(Team &batting, Team &bowling){
    battingSequence.clear();
    bowlingSequence.clear();
    printInningsStart();
    bool match_end = false;
    newBatsman(batting);
    for(int i=0; i<totalOvers; i++){
        vector<string> thisOver;
        newBowler(bowling);
        for(int j=0; j<6; j++){
            batting.battingStarted = true;
            cout<<"\tPress Enter to bowl >> ";
            while(getch() != 13);
            cout<<endl<<"\tBowling...";
            sleep(2);
            int run = playerRun();
            batting.players.at(battingIndex1).ballPlayed++;
            if(run == 7){
                batting.players.at(battingIndex1).out = true;
                bowling.players.at(bowlingIndex).wickets++;
                bowling.players.at(bowlingIndex).bowlingSpell.push_back("W");
                thisOver.push_back("W");
            }else{
                batting.players.at(battingIndex1).runsScored += run;
                bowling.players.at(bowlingIndex).runsGiven += run;
                bowling.players.at(bowlingIndex).bowlingSpell.push_back(to_string(run));
                thisOver.push_back(to_string(run));
            }
            cout<<endl<<endl<<"\t"<<bowling.players.at(bowlingIndex).playerName<<" to "<<batting.players.at(battingIndex1).playerName<<" "<<runs.at(run)<<"!"<<endl<<endl;
            printPerBall(batting, bowling, thisOver);
            if(batting.teamWickets() == totalPlayers-1){
                match_end = true;
                break;
            }
            celebration(batting, bowling);
            if(!firstInnings && batting.teamRuns() > bowling.teamRuns()){
                match_end = true;
                break;
            }
            if(run == 7){newBatsman(batting);}
        }
        lastover_bowled = bowling.players.at(bowlingIndex).playerName;
        if(match_end){break;}
        cout<<"\t"<<batting.teamName<<" scores "<<batting.teamRuns()<<" runs at the end of over "<<ballToOver(bowling.pitchedBalls())<<" for "<<batting.teamWickets()<<" wicket(s)."<<endl<<endl;
        int temp = battingIndex1;
        battingIndex1 = battingIndex2;
        battingIndex2 = temp;
    }
    printInningsEnd();
    inningsResult(batting, bowling);
}

void winCheck(Team t1, Team t2){
    if(t1.teamRuns() > t2.teamRuns()){
        if(current_batting == t1.teamName){
            cout<<"\t"<<t1.teamName<<" won the match by "<<t1.teamRuns()-t2.teamRuns()<<" run(s)!"<<endl;
        }else{
            cout<<"\t"<<t1.teamName<<" won the match by "<<totalPlayers-t1.teamWickets()-1<<" wicket(s) ("<<totalOvers*6-t2.pitchedBalls()<<" ball(s) left)!"<<endl;
        }
    }else if(t1.teamRuns() < t2.teamRuns()){
        if(current_batting == t2.teamName){
            cout<<"\t"<<t2.teamName<<" won the match by "<<t2.teamRuns()-t1.teamRuns()<<" run(s)!"<<endl;
        }else{
            cout<<"\t"<<t2.teamName<<" won the match by "<<totalPlayers-t2.teamWickets()-1<<" wicket(s) ("<<totalOvers*6-t1.pitchedBalls()<<" ball(s) left)!"<<endl;
        }
    }else{
        cout<<"\tMatch draw!"<<endl;
    }
    cout<<endl;
}

void exitGame(){
    cout<<"\tPress ESC to exit the game >> ";
    while(getch() != 27);
    cout<<endl;
    cout<<"\tPress Enter to confirm >> ";
    while(getch() != 13);
}

int main(){
    Team t1,t2;
    t1.teamName = "Team-A";
    t2.teamName = "Team-B";
    startingPage();
    gameInitialisation();
    teamSelection(t1,t2);
    toss(t1,t2);
    for(int i=0; i<2; i++){
        if(current_batting == t1.teamName){
            mainGame(t1,t2);
            current_batting = t2.teamName;
            current_bowling = t1.teamName;
        }else{
            mainGame(t2,t1);
            current_batting = t1.teamName;
            current_bowling = t2.teamName;
        }
        firstInnings = false;
    }
    winCheck(t1,t2);
    cout<<"\tPlease wait a little. We are making the match summary ready...";
    sleep(5);
    cout<<endl<<endl;
    matchSummary(t1,t2);
    cout<<endl;
    exitGame();
    return 0;
}