#pragma once

#include<cstring>
#include<vector>
using namespace std;

#define BOARD_SIZE 8
#define EMPTY 0
#define BLACK 1
#define WHITE -1

#define OTHER(p) (-p)

#define BOUNDS(x,y) (x>=0 && x<BOARD_SIZE && y>=0 && y<BOARD_SIZE)

#define PASS -1

const int DIR[8][2] = {{0,1},{1,0},{0,-1},{-1,0},{1,1},{1,-1},{-1,-1},{-1,1}};

struct GameState{
    int board[BOARD_SIZE][BOARD_SIZE];
    int player;
    bool passed;

    GameState():
        player(BLACK){
        fill(board[0], board[BOARD_SIZE-1]+BOARD_SIZE, EMPTY);
        passed = false;
        int half = BOARD_SIZE/2-1;
        board[half][half] = WHITE;
        board[half+1][half+1] = WHITE;
        board[half+1][half] = BLACK;
        board[half][half+1] = BLACK;
    }

    GameState(const GameState& other){
        player = other.player;
        passed = other.passed;
        copy(other.board[0], other.board[BOARD_SIZE-1]+BOARD_SIZE, board[0]);
    }

    void apply(const int p){
        if(p==PASS){
            passed = true;
        } else{
            passed = false;
            int i=p/BOARD_SIZE, j=p%BOARD_SIZE;
            for(int dir=0; dir<8; dir++){
                int ci = i+DIR[dir][0];
                int cj = j+DIR[dir][1];
                while(BOUNDS(ci,cj) && board[ci][cj]==OTHER(player)){
                    ci += DIR[dir][0];
                    cj += DIR[dir][1];
                }
                if(ci==i+DIR[dir][0] && cj==j+DIR[dir][1])continue;
                if(BOUNDS(ci,cj) && board[ci][cj]==player){
                    while(ci!=i || cj!=j){
                        ci -= DIR[dir][0];
                        cj -= DIR[dir][1];
                        board[ci][cj] = player;
                    }
                }
            }
        }
        player = OTHER(player);
    }

    vector<int> moves(){
        vector<int> res;
        for(int i=0; i<BOARD_SIZE; i++){
            for(int j=0; j<BOARD_SIZE; j++){
                if(board[i][j] == EMPTY){
                    bool check = false;
                    for(int dir=0; dir<8; dir++){
                        int ci = i+DIR[dir][0];
                        int cj = j+DIR[dir][1];
                        while(BOUNDS(ci,cj) && board[ci][cj]==OTHER(player)){
                            ci += DIR[dir][0];
                            cj += DIR[dir][1];
                        }
                        if(ci==i+DIR[dir][0] && cj==j+DIR[dir][1])continue;
                        if(BOUNDS(ci,cj) && board[ci][cj]==player){
                            check = true;
                            break;
                        }
                    }
                    if(check){
                        res.push_back(i*BOARD_SIZE+j);
                    }
                }
            }
        }
        return res;
    }

    void print(){
        vector<int> valid_moves = moves();
        int cur = 0;
        for(int i=0; i<BOARD_SIZE; i++){
            for(int j=0; j<BOARD_SIZE; j++){
                int val = board[i][j];
                if(val == EMPTY){
                    if(cur<valid_moves.size() && valid_moves[cur]==i*BOARD_SIZE+j){
                        cout<<'+';
                        cur++;
                    }
                    else cout<<'.';
                }
                else if(val == BLACK)cout<<'B';
                else if(val == WHITE)cout<<'W';
            }
            cout<<'\n';
        }
        if(player==BLACK)cout<<"BLACK TURN\n";
        else if(player==WHITE)cout<<"WHITE TURN\n";
    }

    int eval_pieces(int player_){
        int cnt = 0;
        for(int i=0; i<BOARD_SIZE; i++){
            for(int j=0; j<BOARD_SIZE; j++){
                if(board[i][j] == player_)cnt++;
            }
        }
        return cnt;
    }

    int winner(){
        int diff = 0;
        for(int i=0; i<BOARD_SIZE; i++){
            for(int j=0; j<BOARD_SIZE; j++){
                if(board[i][j] == BLACK)diff++;
                else if(board[i][j] == WHITE)diff--;
            }
        }
        if(diff > 0)return BLACK;
        if(diff < 0)return WHITE;
        return EMPTY;
    }
};
