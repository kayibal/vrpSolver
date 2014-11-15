//
//  Solver.h
//  vrpSolver
//
//  Created by Alan Höng on 14/11/14.
//  Copyright (c) 2014 Alan Höng. All rights reserved.
//

#ifndef __vrpSolver__Solver__
#define __vrpSolver__Solver__

#include <iostream>
#include "Solution.h"

class Solver{
    //Johnson et al 89-91 p141
    float temp_init;
    float temp_factor;
    float cutoff;
    int size_factor;
    int find_divisor;
    int neighborhood_size;

    int max_time;
    /*std::vector<int>*/ int capacity;
    std::vector<node*> nodes;
    Solution best;
    Solution current;
    
    Solution visitNeighbor();
    
public:
    Solver(int s, std::string file);
    void start();
    void printSolution();
};

#endif /* defined(__vrpSolver__Solver__) */
