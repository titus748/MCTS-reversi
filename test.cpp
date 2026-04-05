#include<iostream>
#include<cstdlib>
#include<ctime>

#include"game.h"
#include"basic.h"
#include"uct.h"
#include"ucb.h"

using namespace std;

#define N_ROLLOUT 1000
#define N_TEST 100

int main(){
    //srand(time(0));
    srand(1);
    int black = 0, white=0, draw=0;
    for(int r=1; r<=N_TEST; r++){
        cout<<"ROUND"<<r<<'\n';
        GameState state;
        bool passed = false;
        while(1){
            //state.print();
            /*int black=0, white=0, draw=0;
            for(int i=0; i<N_ROLLOUT; i++){
                int res = rollout_game(&state);
                if(res == BLACK)black++;
                else if(res == WHITE)white++;
                else draw++;
            }
            cout<<"_____________________\n";
            cout<<"black win rate: "<<double(black)/N_ROLLOUT<<"\n";
            cout<<"white win rate: "<<double(white)/N_ROLLOUT<<"\n";
            cout<<"draw rate     : "<<double(draw)/N_ROLLOUT<<"\n";
            cout<<"=====================\n";*/
            bool pass;
            if(state.player == BLACK)pass = !io_move(&state); // black player
            else pass = !uct_move(&state, 100, 0); // white player
            if(pass){
                if(passed)break;
                passed = true;
            } else passed = false;
        }
        int res = state.winner();
        if(res == BLACK)cout<<"BLACK WINS\n";
        else if(res == WHITE)cout<<"WHITE WINS\n";
        else cout<<"DRAW\n";
        cout<<"BLACK: "<<state.eval_pieces(BLACK)<<'\n';
        cout<<"WHITE: "<<state.eval_pieces(WHITE)<<'\n';
        if(res == BLACK)black++;
        else if(res == WHITE)white++;
        else draw++;
        cout<<"black win rate: "<<double(black)/r<<"\n";
        cout<<"white win rate: "<<double(white)/r<<"\n";
        cout<<"draw rate     : "<<double(draw)/r<<"\n";
        cout<<"=========================\n";
    }
    cout<<"black win rate: "<<double(black)/N_TEST<<"\n";
    cout<<"white win rate: "<<double(white)/N_TEST<<"\n";
    cout<<"draw rate     : "<<double(draw)/N_TEST<<"\n";
}
