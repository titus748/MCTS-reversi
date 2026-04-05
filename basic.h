#pragma once

#include<iostream>
#include<vector>
#include<string>

#include"game.h"

using namespace std;

bool io_move(GameState* state){
    state->print();
    auto valid_moves = state->moves();
    if(valid_moves.size() == 0){
        cout<<"Pass check: ";
        string s;
        cin>>s;
        state->apply(PASS);
        return false;
    }
    int p;
    while(1){
        bool check = false;
        cout<<"Enter your move(i[1~"<<BOARD_SIZE<<"],j[1~"<<BOARD_SIZE<<"]): ";
        int i, j;
        cin>>i>>j;
        if(i>=1 && i<=BOARD_SIZE && j>=1 && j<=BOARD_SIZE){
            p = (i-1)*BOARD_SIZE+j-1;
            for(int k=0; k<valid_moves.size(); k++){
                if(valid_moves[k] == p){
                    check = true;
                    break;
                }
            }
        }
        if(check)break;
    }
    state->apply(p);
    return true;
}

bool random_move(GameState* state){
    auto valid_moves = state->moves();
    if(valid_moves.size() == 0){
        state->apply(PASS);
        return false;
    }
    state->apply(valid_moves[rand()%valid_moves.size()]);
    return true;
}

int rollout_game(GameState* state){
    GameState cur(*state);
    bool passed = false;
    while(1){
        if(!random_move(&cur)){
            if(passed)break;
            passed = true;
        } else passed = false;
    }
    return cur.winner();
}
