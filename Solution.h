//
//  Solution.h
//  vrpSolver
//
//  Created by Alan Höng on 14/11/14.
//  Copyright (c) 2014 Alan Höng. All rights reserved.
//

#ifndef __vrpSolver__Solution__
#define __vrpSolver__Solution__

#include <iostream>
#include <vector>

typedef struct {
    int node_id;
    int time;
    int demand;
    double x;
    double y;
} node;

class Solution{
    std::vector<node*> routes;
    float value;
    
public:
    bool debug;
    /*
     Constructor assumes that first node in array is depot
     */
    Solution(std::vector<node*>& nodes);
    Solution(Solution& s);
    Solution();
    
    void one2one(int a, int b);
    void deleteInsert(int a, int b);
    void partialReverse(int a, int b);
    
    float evaluate();
    int getSize();
    bool isFeasable(int max_time, int capacity);
    
    void print();
};

#endif /* defined(__vrpSolver__Solution__) */
