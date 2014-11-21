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
#include <ctime>
#include <fstream>

using namespace std;

int main(int argc, const char * argv[])
{
    //standard values for SA
    int file_index[5] = {1,2,6,14,10};
    float optimal[5][3] = {{524.6,      548.15,     547.94},
                           {835.3,      869.54,     878.65},
                           {555.4,      557.45,     555.4},
                           {1395.8,     1450.37,    1454.0},
                           {886.4,      912.9,      973.26}};
    float temp_init[3] = {10, 15, 20};
    float temp_factor[3] = {0.5, 0.75, 0.95};
    float cutoff[3] = {0.1,0.25,0.5};
    int size_factor[3] = {4,8,12};
    int find_divisor[3] = {20,30,50};
    
    clock_t t;
    float sec;
    std::string path = "/Users/Alan/Downloads/instances/Christofides_";
    
    int seed = 0xDEADBEEF;
    
    ofstream out("/Users/Alan/Downloads/instances/results.latex");
    out.setf(ios::fixed,ios::floatfield);
    out.precision(2);

    for (int i = 0; i < 4; i++){
        
        std::string file = path+to_string(file_index[i])+".vrp";
        
        out << "%" << file << "\n";
        
        for (int k = 0; k < 3; k++){
            out << "%S" << k << ":\n";
            out << "\\begin{tabular}{| l | l | l | l | l || l | l | l | l | l | l | }\n";
            out << "\\hline\n";
            out << "TI & TF & CO & SR & FD & SI & SF & SI2SF & OPT & TIME & SOLVER\\\\\n";
            out << "\\hline\n";
            for(int j = 0; j < 3; j++){
                cout << "at: " << i << ", " << k << ", " << j << "\n";
                Solver s(seed, file ,k+1, temp_init[j], temp_factor[j], cutoff[j], size_factor[j], find_divisor[j]);
                
                t = clock();
                
                s.start();
                
                t = clock() - t;
                sec = ((float)t)/CLOCKS_PER_SEC;
                
                Solution a;
                a = s.getInit();
                
                Solution e;
                e = s.getSolution();
                
                float p1 = (a.evaluate()-e.evaluate())*100/a.evaluate();
                float p2 = e.evaluate()*100/optimal[i][k];
                
                //"TI & TF & CO & SR & FD & SI & SF & %SI2SF & %OPT & TIME & SOLVER"
                out << temp_init[j] << " & " << temp_factor[j] << " & " << cutoff[j] << " & " << size_factor[j] << " & " << find_divisor[j] << " & ";
                out << a.evaluate() << " & " <<  e.evaluate() << " & " << p1 << " & " << p2 << " & " << sec << " & " << "N/A" << "\\\\\n" ;
                out << "\\hline\n";
            }
            out << "\\end{tabular}\n\n\n";
            out.flush();
        }
    }

    // insert code here...
    Solver s(8132947,"../Christofides_10.vrp");
    s.start();
    return 0;
}

