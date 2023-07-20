#include "Engine.h"


//Evaluate Hand and Make Decision for 5 rounds of play (pre-flop, flop, turn, river)
//Pre-flop take and store dealt hand as input
//Update Community Cards with each round
//MAX CAPACITY SIMULATIONS IS 25,000

const unsigned int NUM_SIMULATIONS = 10000;

string rounds[4] = {"Pre-Flop", "Flop", "Turn", "River"};
vector<string> suits = {"Hearts", "Spades", "Diamonds", "Clubs"};
vector<string> cards = {"2", "3", "4", "5", "6", "7", 
                        "8", "9", "Ten", "Jack", "Queen", "King", "Ace"};


unordered_set<string> deck_of_cards;
vector<string> community_cards;

int main(){
    srand(time(0));
    //Create a 52 card deck
    build_deck(suits, cards, deck_of_cards);
    

    cout<<"What are your first two cards?"<<endl;
    string one, two;
    string a,b,c,d,e,f;
    cin>>a>>b>>c>>d>>e>>f;
    one = a+ " "+b+" "+c;
    two = d+" "+e+" "+f;
    Player user(one, two, 2000.00, 10);
    deck_of_cards.erase(one);
    deck_of_cards.erase(two);
    
    cout<<"What is your current bankroll?"<<endl;
    double br;
    cin>>br;
    //Game Begins
    bankroll Player(br);
    double pot_size , total_bet;
    cout<<"What is the current Pot Size?"<<endl;
    cin>>pot_size;
    cout<<"What is your Total Risk?"<<endl;
    cin>>total_bet;
    cout<<endl;
    
    Player.pot_size = pot_size;
    Player.total_bet = total_bet;
    
    int i = 0;
    while(i<sizeof(rounds)/sizeof(string)){
        cout<<rounds[i]<<" "; // Begin each round

        if(i == 1){//flop

            //Take input
            for(int k = 0;k<3;k++){
                string s,b;
                for(int j = 0;j<3;j++){
                    cin>>b;
                    if(j>0){
                        s = s+" "+b;
                    }
                    else s+=b;   
                }
                deck_of_cards.erase(s);
                community_cards.push_back(s);
            }
        }
        else if(i>1){// Turn and River
            
            //Take community cards and update array for Turn and River rounds;
            string a,b,c;
            cin>>a>>b>>c;
            deck_of_cards.erase(a+" "+b+" "+c);
            community_cards.push_back(a+" "+b+" "+c);            
        } 
        simulate_rates(NUM_SIMULATIONS, user,community_cards, deck_of_cards, pot_size, total_bet, Player);
        
        //take in bets from all players
        //update balances
        cout<<"Have new bets been made? Enter Y or N"<<endl;
        string round_over;
        cin>>round_over;
        while(round_over!= "N" && round_over == "Y"){
            cout<<"What is the new Pot Size?"<<endl;
            cin>>pot_size;
            cout<<"What is your new total risk?"<<endl;
            cin>>total_bet;
            cout<<endl;
            simulate_rates(NUM_SIMULATIONS, user,community_cards, deck_of_cards, pot_size, total_bet, Player);
            cout<<"Have new bets been made? Enter Y or N"<<endl;
            cin>>round_over;
        }
        
        cout<<endl;
        i++;
    }
    
    

    return 0;
}



