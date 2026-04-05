#pragma once

#include<vector>
#include<cmath>
#include<time.h>

#include"game.h"
#include"basic.h"

using namespace std;

struct TreeNode{
    int n_visited;
    vector<int> win;
    vector<int> loss;
    vector<int> n;
    vector<int> valid_moves;
    vector<TreeNode*> children;
    TreeNode* pass_node;
    GameState* state;

    TreeNode(GameState* state):
        pass_node(NULL), state(state){
        valid_moves = state->moves();
        n_visited = 0;
        win.resize(valid_moves.size());
        loss.resize(valid_moves.size());
        n.resize(valid_moves.size());
        children.resize(valid_moves.size());
    }

    ~TreeNode(){
        delete state;
        for(auto child: children)
            if(child)delete child;
        if(pass_node)delete pass_node;
    }

    int select_best(){
        double best_score = -1, score;
        int best_move;
        for(int i=0; i<valid_moves.size(); i++){
            score = double(win[i])/n[i];
            if(score > best_score){
                best_score = score;
                best_move = i;
            }
        }
        return valid_moves[best_move];
    }

    int update(){
        int winner = -1;
        int next_move = -1;
        if(valid_moves.size() == 0){
            if(state->passed)return state->winner();
            if(!pass_node){
                GameState* pass_state = new GameState(*state);
                pass_state->apply(PASS);
                pass_node = new TreeNode(pass_state);
            }
            return pass_node->update();
        }
        if(n_visited < valid_moves.size()){
            next_move = n_visited++;
            GameState* next_state = new GameState(*state);
            next_state->apply(valid_moves[next_move]);
            children[next_move] = new TreeNode(next_state);
            winner = rollout_game(children[next_move]->state);
        } else{
            double max_val = -1;
            for(int i=0; i<valid_moves.size(); i++){
                double val = double(win[i])/n[i] + sqrt(2*log(n_visited)/n[i]);
                if(val > max_val){
                    max_val = val;
                    next_move = i;
                }
            }
            winner = children[next_move]->update();
        }
        n[next_move]++;
        if(winner == state->player)win[next_move]++;
        else if(winner == OTHER(state->player))loss[next_move]++;
        return winner;
    }
};

bool uct_move(GameState* state, int n_trials, int run_time){
    clock_t st, en;
    st = clock();
    auto valid_moves = state->moves();
    if(valid_moves.size() == 0){
        state->apply(PASS);
        return false;
    }
    GameState* root_state = new GameState(*state);
    TreeNode root(root_state);
    if(run_time > 0){
        while(true){
            root.update();
            en = clock();
            if(double(en-st)/CLOCKS_PER_SEC > run_time)break;
        }
    } else{
        for(int i=0; i<valid_moves.size()*n_trials; i++){
            root.update();
        }
    }
    int best = root.select_best();
    state->apply(best);
    return true;
}
