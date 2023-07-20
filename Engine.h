#include "Player.h"
#include <random>
#include <memory>
#include <queue>
#include <utility>
#include <iomanip>

// Implement a bankroll management system to guide decision-making
struct bankroll{
    double pot_size = 0.00, total_bet = 0.00, bank;
    bankroll(double b){
        bank = b;
    }
    void max_edge(double reward, double risk, double win_rate, double loss_rate){
        if(risk == 0 || reward == 0) return;
        double K_Criterion = (win_rate/risk)-(loss_rate/abs(reward-risk));
        cout<<endl<<"Recommendation: ";
        K_Criterion/=2; // Half the percentage to take a more conservative approach to bet sizing
        if(K_Criterion <=0) cout<<"Fold"<< endl;
        else if(K_Criterion < 1) cout<<"Bet up to a maximum of $"<<(int)(K_Criterion*bank)<<endl;
        else if(K_Criterion >= 1) cout<<"All In"<<endl;
    }
};

//USE TO SORT A HAND
unordered_map<char,int> card_rank = {
    {'A', 14}, {'K', 13}, {'Q', 12}, {'J', 11}, {'T', 10}, {'9', 9}, {'8', 8}, {'7',7},
    {'6', 6}, {'5', 5}, {'4', 4}, {'3', 3}, {'2', 2}
};
bool compare(string one, string two){
    return card_rank[one[0]]<card_rank[two[0]];
}

string getCard(string s){
    int i = 0;
    while(s[i]!=' ') i++;
    return s.substr(0,i);
}
string getSuit(string s){
    int i = s.size()-1;
    while(s[i]!=' ') i--;
    return s.substr(i+1,s.size());
}

void RoyalFlush(int& s, vector<string> h){
    if(s>1000) return;

    vector<string> sp, cl, he, di;
    for(auto a: h){
        string b = getSuit(a);
        if(b == "Spades") sp.push_back(a);
        else if(b == "Clubs") cl.push_back(a);
        else if(b == "Diamonds") di.push_back(a);
        else he.push_back(a);
    }

    if(sp.size() >=5) h = sp;
    else if(cl.size()>=5) h = cl;
    else if(di.size()>=5) h = di;
    else if(he.size()>=5) h = he;
    else return;
    
    for(int i = 0;i<3;i++){
        if(h[i][0] == 'T'){
            if(h[i+1][0] == 'J'){
                if(h[i+2][0] == 'Q'){
                    if(h[i+3][0] == 'K'){
                        if(h[i+4][0] == 'A'){
                            s = max(s, 1014);
                            return;
                        } 
                    }
                }
            }
        }
    }
}
void StraightFlush(int& s, vector<string> h){
    if(s>900) return;
    vector<string> sp, cl, he, di;
    for(auto a: h){
        string b = getSuit(a);
        if(b == "Spades") sp.push_back(a);
        else if(b == "Clubs") cl.push_back(a);
        else if(b == "Diamonds") di.push_back(a);
        else he.push_back(a);
    }

    if(sp.size() >=5) h = sp;
    else if(cl.size()>=5) h = cl;
    else if(di.size()>=5) h = di;
    else if(he.size()>=5) h = he;
    else return;

    for(int i = h.size()-5;i>=0;i--){
        if(card_rank[h[i][0]] +1 == card_rank[h[i+1][0]]){
            if(card_rank[h[i+1][0]] +1 == card_rank[h[i+2][0]]){
                if(card_rank[h[i+2][0]] +1 == card_rank[h[i+3][0]]){
                    if(card_rank[h[i+3][0]] +1 == card_rank[h[i+4][0]]){
                        s = max(s, 900 + card_rank[h[i+4][0]]);
                        return;
                    }
                }
            }
        }
    }
}
void FourKind(int& s, vector<string> h){
    if(s>800) return;
    for(int i = 3;i>=0;i--){ 
        if(getCard(h[i]) == getCard(h[i+1])){
            if(getCard(h[i+1]) == getCard(h[i+2])){
                if(getCard(h[i+2]) == getCard(h[i+3])){
                    s = max(s, 800 + card_rank[h[i][0]]);
                    return;
                }
            }
        }
    }
}
void FullHouse(int& s, vector<string> h){
    if(s>700) return;
    bool pair =false, triple = false;
    int kicker = 0;
    for(int i = 6;i>0;i--){
        if(i-2>=0 && h[i][0] == h[i-1][0] && h[i][0] == h[i-2][0] && !triple){
            triple = true;
            kicker = max(kicker, card_rank[h[i][0]]);
            i -=2;
        }
        else if(h[i][0] == h[i-1][0]){
            pair = true;
            kicker = max(kicker, card_rank[h[i][0]]);
            i-=2;
        }
        if(pair && triple){
            s = max(s, 700 + kicker);
            return;
        } 
    }
}
void Flush(int& s, vector<string> h){
    if(s>600) return;
    pair<int, int> clubCount = {0,0}, spadeCount = {0,0}, heartCount = {0,0}, diamondCount = {0,0};
    for(int i = 0;i<h.size();i++){
        string s = getSuit(h[i]);
        if(s == "Clubs"){
            clubCount.first++;
            clubCount.second = max(clubCount.second, card_rank[h[i][0]]);
        }
        else if(s == "Spades"){
            spadeCount.first++;
            spadeCount.second = max(spadeCount.second, card_rank[h[i][0]]);
        }
        else if(s == "Diamonds"){
            diamondCount.first++;
            diamondCount.second = max(diamondCount.second, card_rank[h[i][0]]);
        }
        else if(s == "Hearts"){
            heartCount.first++;
            heartCount.second = max(heartCount.second, card_rank[h[i][0]]);
        }
    }
    
    if(clubCount.first >= 5) s = max(s, 600 + clubCount.second);
    else if(spadeCount.first >= 5) s = max(s, 600 + spadeCount.second);
    else if(diamondCount.first >= 5) s = max(s, 600 + diamondCount.second);
    else if(heartCount.first >=5) s = max(s, 600 + heartCount.second);
    else return;
}
void Straight(int& s, vector<string> h){ 
    if(s>500) return;

    for(int i = 0;i<h.size()-1;i++){
        if(h[i][0] == h[i+1][0]) h.erase(h.begin()+i);
    }//Remove Duplicates
    
    for(int i = 2;i>=0;i--){
        if(card_rank[h[i][0]] +1 == card_rank[h[i+1][0]]){
            if(card_rank[h[i+1][0]] +1 == card_rank[h[i+2][0]]){
                if(card_rank[h[i+2][0]] +1 == card_rank[h[i+3][0]]){
                    if(card_rank[h[i+3][0]] +1 == card_rank[h[i+4][0]]){
                        s = max(s, 500 + card_rank[h[i+4][0]]);
                        return;
                    }
                }
            }
        }
    }
}
void ThreeKind(int& s, vector<string> h){
    if(s>400) return;
    for(int i = 4;i>=0;i--){
        if(h[i][0] == h[i+1][0] && h[i][0] == h[i+2][0]){
            s = max(s, 400+card_rank[h[i][0]]);
            return;
        }
    }
}
void TwoPair(int& s, vector<string> h){
    if(s>300) return;
    bool one =false, two = false;
    int kicker = 0;
    for(int i = 5;i>=0;i--){
        if(h[i][0] == h[i+1][0]){
            if(!one) one = true;
            else two = true;
            kicker = max(kicker, card_rank[h[i][0]]);
            i-=2;
        }
        if(one && two){
            s = max(s, 300 + kicker);
            return;
        }  
    }
}
void Pair(int& s, vector<string> h){
    if(s>200) return;
    for(int i = 5;i>=0;i--){
        if(h[i][0] == h[i+1][0]){
            s = max(s, 200+card_rank[h[i][0]]);
            return;
        }
    }
}
void HighCard(int& s, vector<string> h){
    if(s>100) return;
    s =  max(s, 100 + card_rank[h.back()[0]]);
    return;
}




//Gauge if the user wins the hand or not
bool win_or_lose(Player user, vector<vector<string>> other_players,
            vector<string> community_cards){
    priority_queue<int> q;
    /*
    HAND HIERARCHY
    1. Royal Flush (1000 pts)
    2. Straight Flush (900 pts)
    3. Four of a kind (800 pts)
    4. Full House (700 pts)
    5. Flush (600 pts)
    6. Straight (500 pts)
    7. Three of a Kind (400 pts)
    8. Two Pair (300 pts)
    9. Pair (200 pts)
    10. High Card (100 pts)
    */
    for(int i = 0;i<=other_players.size();i++){
        int score = -1;
        vector<string> h = community_cards;
        if(i == other_players.size()){
            h.push_back(user.get_hand()[0]);
            h.push_back(user.get_hand()[1]);
        } 
        else{
            h.push_back(other_players[i][0]);
            h.push_back(other_players[i][1]);
        } 
        sort(h.begin(),h.end(), compare);
        RoyalFlush(score, h);
        StraightFlush(score, h);
        FourKind(score, h);
        FullHouse(score, h);
        Flush(score, h);
        Straight(score, h);
        ThreeKind(score, h);
        TwoPair(score, h);
        Pair(score, h);
        HighCard(score, h);
            
        if(i == other_players.size()){
            user.points = score;
        }
        q.push(score);
    }   
    if(user.points == q.top())return true;
    return false;
}



// draw a random card from the deck
string draw_card(unordered_set<string>& deck_of_cards){
    int a = rand()%deck_of_cards.size();
    auto it = deck_of_cards.begin();
    advance(it, a);
    string ans = *it;
    deck_of_cards.erase(*it);
    return ans;
}

// build a 52-card deck
void build_deck(vector<string> suits, vector<string> cards, unordered_set<string>& deck_of_cards){
    for(int i = 0;i<suits.size();i++){
        for(int j = 0;j<cards.size();j++){
        deck_of_cards.insert((cards[j]+ " of "+ suits[i]));
        }
    }  
}

// Calculate EV to make a play
double calculate_EV(double pot, double total_bet, double win_chance, double loss_chance){
    cout<<" Pot Size: "<<pot<<" | Total Bet: "<<total_bet<<" | "<<"Expected Value: ";
    return (pot*win_chance) - (total_bet * loss_chance);
}

/* Calculate the chance of winning and losing each round by conducting monte carlo simulations of 
current game conditions */
void simulate_rates(int num_sims,Player user,  vector<string> community_cards, 
                    unordered_set<string>deck_of_cards, double pot, double bet, bankroll Player){
    
    double num_wins = 0.0, num_losses = 0.0, i = num_sims;
    vector<vector<string>> other_players;
    vector<string> temp;
    string one,two;
    unordered_set<string> temp_deck;
    while(i--){
        temp_deck = deck_of_cards;
        for(int i = 0;i<4;i++){
            string one = draw_card(temp_deck);
            string two = draw_card(temp_deck);
            other_players.push_back({one, two});
        }
        temp = community_cards;
        
        while(temp.size() != 5){
            temp.push_back(draw_card(temp_deck));
        }
        
        if(win_or_lose(user, other_players, temp)) num_wins++;
        else num_losses++;
        other_players.clear();
    }

    cout<<setprecision(4)<<num_sims<<" simulations"<<endl<<endl<<"Win: "<<(num_wins*100)/num_sims<<"% | Lose: "
        <<(num_losses*100)/num_sims<<"% |"<<calculate_EV(pot,bet, num_wins/num_sims, num_losses/num_sims)<<endl;
    Player.max_edge(pot, bet, num_wins/num_sims, num_losses/num_sims);
}


