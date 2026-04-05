#pragma once

#include<cmath>
#include<ctime>

#include"game.h"
#include"basic.h"

using namespace std;

bool ucb1_move(GameState* state, int n_trials, int run_time){
    clock_t st, en;
    st = clock();
    auto valid_moves = state->moves();
    if(valid_moves.size() == 0){
        state->apply(PASS);
        return false;
    }
    int player = state->player;
    vector<int> win(valid_moves.size());
    vector<int> loss(valid_moves.size());
    vector<int> n(valid_moves.size());
    if(run_time > 0){
        int i = 0;
        while(true){
            int max_j = -1;
            double max_val = -1;
            for(int j=0; j<valid_moves.size(); j++){
                if(n[j]==0){
                    max_j = j;
                    break;
                }
                double val = double(win[j])/n[j] + sqrt(2*log(i)/n[j]);
                if(val > max_val){
                    max_val = val;
                    max_j = j;
                }
            }
            GameState next_state(*state);
            next_state.apply(valid_moves[max_j]);
            int res = rollout_game(&next_state);
            n[max_j]++;
            if(res == player)win[max_j]++;
            else if(res == OTHER(player))loss[max_j]++;
            i++;
            en = clock();
            if(double(en-st)/CLOCKS_PER_SEC > run_time)break;
        }
    } else{
        for(int i=0; i<valid_moves.size()*n_trials; i++){
            int max_j = -1;
            double max_val = -1;
            for(int j=0; j<valid_moves.size(); j++){
                if(n[j]==0){
                    max_j = j;
                    break;
                }
                double val = double(win[j])/n[j] + sqrt(2*log(i)/n[j]);
                if(val > max_val){
                    max_val = val;
                    max_j = j;
                }
            }
            GameState next_state(*state);
            next_state.apply(valid_moves[max_j]);
            int res = rollout_game(&next_state);
            n[max_j]++;
            if(res == player)win[max_j]++;
            else if(res == OTHER(player))loss[max_j]++;
        }
    }
    int best_move = -1;
    double best_score = -1, score;
    for(int i=0; i<valid_moves.size(); i++){
        score = double(win[i]) / n[i];
        if(score > best_score){
            best_score = score;
            best_move = i;
        }
    }
    state->apply(valid_moves[best_move]);
    return true;
}
