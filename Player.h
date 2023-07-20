#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <unordered_set>
#include <algorithm>
using namespace std;

//Create a game state class that keeps track of 5 players
//Create a player class that keeps track of a player's hands
//Make a decision on which hand wins


class Player{
    vector<string> starting_hand;
    double balance, total_bet;
    
    public:
    int points = 0;
    
    Player(string card_one, string card_two, double b, double tb){
        starting_hand.push_back(card_one);
        starting_hand.push_back(card_two);
        balance = b;
        total_bet = tb;
    }
    vector<string> get_hand(){
        return starting_hand;
    }
};
