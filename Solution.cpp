//
//  Solution.cpp
//  vrpSolver
//
//  Created by Alan Höng on 14/11/14.
//  Copyright (c) 2014 Alan Höng. All rights reserved.
//

#include "Solution.h"
#include <math.h>

Solution::Solution(std::vector<node*> nodes){
    value = 0;
    node* depot = nodes[0];
    routes.resize(nodes.size()*2);
    for (int i = 1; i < nodes.size()-1; i++){
        routes[i] = depot;
        routes[i+1] = nodes[i];
    }
}

Solution::Solution(Solution& s){
    value = 0;
    routes = s.routes;
}

Solution::Solution(){
    value = 0;
}

void Solution::one2one(int a, int b){
    node* temp = routes[a];
    routes[a] = routes[b];
    routes[b] = temp;
}

void Solution::deleteInsert(int a, int b){
    node* temp = routes[b];
    routes.erase(routes.begin() + b);
    routes.insert(routes.begin()+a, temp);
}

void Solution::partialReverse(int a, int b){
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

int Solution::evaluate(){
    value = 0;
    for (int i = 0; i < routes.size(); i++){
        value += sqrt(pow((routes[i]->x * routes[i+1]->x),2) + pow((routes[i]->y - routes[i+1]->y),2))+routes[i]->time;
    }
    return value;
}