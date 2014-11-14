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
    int seed;
    int max_time;
    /*std::vector<int>*/ int capacity;
    std::vector<node*> nodes;
    Solution best;
    Solution current;
    
    Solution visitNeighbor();
    
public:
    Solver(int s, int max_t, std::string file);
    void start();
    float calcHeat();
};

#endif /* defined(__vrpSolver__Solver__) */
