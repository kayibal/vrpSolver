//
//  Solution.cpp
//  vrpSolver
//
//  Created by Alan Höng on 14/11/14.
//  Copyright (c) 2014 Alan Höng. All rights reserved.
//

#include "Solution.h"
#include <math.h>

Solution::Solution(std::vector<node*>& nodes){
    value = 0;
    node* depot = nodes[0];
    routes.resize((nodes.size()-1)*2);
    debug = false;
    
    for (int i = 0; i < nodes.size()-1; i++){
        routes[2*i] = depot;
        routes[2*i+1] = nodes[i+1];
    }
}

Solution::Solution(Solution& s){
    value = 0;
    debug = false;
    routes = s.routes;
}

Solution::Solution(){
    value = 0;
    debug = false;
}

void Solution::one2one(int a, int b){
    if(a == 0 || b == 0){
        return;
    }
    node* temp = routes[a];
    routes[a] = routes[b];
    routes[b] = temp;
}

void Solution::deleteInsert(int a, int b){
    if(a == 0 || b == 0){
        return;
    }
    node* temp = routes[b];
    routes.erase(routes.begin() + b);
    routes.insert(routes.begin()+a, temp);
}

void Solution::partialReverse(int a, int b){
    if(a == 0 || b == 0){
        return;
    }
    if ( a > b){
        int temp = b;
        b = a;
        a = temp;
    }
    int limit = (b-a)/2;
    for(int i = 0; i < limit; i++){
        node* temp = routes[a+i];
        routes[a+i] = routes[b-i];
        routes[b-i] = temp;
    }
}

int Solution::getSize(){
    return (int)routes.size();
}

//returns total time
float Solution::evaluate(){
    value = 0;
    int next;
    for (int i = 0; i < routes.size(); i++){
        next = (i+1)%(routes.size()); // wraps arround at the end
        value += sqrt(pow((routes[i]->x - routes[next]->x),2) + pow((routes[i]->y - routes[next]->y),2))+routes[i]->time;
    }
    return value;
}

bool Solution::isFeasable(int max_time, std::vector<int> capacity){
    float time = 0;
    std::vector<float> demand(2);
    demand[0] = 0;
    demand[1] = 0;
    int next;
    for (int i = 0; i< routes.size(); i++){
        next = (i+1)%(routes.size()); // wraps arround at the end
        
        time += sqrt(pow((routes[i]->x - routes[next]->x),2) + pow((routes[i]->y - routes[next]->y),2))+routes[i]->time;
        
        demand[0] += routes[i]->demand[0];
        demand[1] += routes[i]->demand[1];
        //each route must be smaller than max_time
        if(time > max_time){
            return false;
        }
        //each routes demand must be smaller than Capacity
        if(demand[0] > capacity[0] || demand[1] > capacity[1]){
            return false;
        }
        if (routes[next]->node_id == 0){
            time = 0;
            demand[0] = 0;
            demand[1] = 0;
        }
    }
    return true;
}
void Solution::print(){
    int last_id = 0;
    int route_count = 1;
    std::cout << "shortest trip time: "<< value;
    for(int i = 1; i<routes.size(); i++){
        if(last_id == 0 && routes[i]->node_id != 0){
            std::cout << "\n";
            std::cout <<"route #" << route_count << ":  " << routes[i]->node_id << " -> ";
            route_count++;
        }
        if (last_id != 0 && routes[i]->node_id != 0){
            std::cout << routes[i]->node_id << " -> ";
        }
            
        last_id = routes[i]->node_id;
        
    }
}