//
//  main.cpp
//  vrpSolver
//
//  Created by Alan Höng on 14/11/14.
//  Copyright (c) 2014 Alan Höng. All rights reserved.
//

#include <iostream>
#include "Solver.h"
#include <climits>
using namespace std;

int main(int argc, const char * argv[])
{

    // insert code here...
    Solver s(8132947,INT_MAX,"/Users/Alan/Downloads/instances/Christofides_1.vrp");
    s.start();
    return 0;
}

