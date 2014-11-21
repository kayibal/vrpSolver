//
//  Solver.cpp
//  vrpSolver
//
//  Created by Alan Höng on 14/11/14.
//  Copyright (c) 2014 Alan Höng. All rights reserved.
//

#include "Solver.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <climits>
using namespace std;

std::string trim(std::string& s)
{
    int p = s.find_first_not_of(" \t\r");
    s.erase(0,p);
    p = s.find_last_not_of(" \t\r");
    if(p!=string::npos)
    {
        s.erase(p+1);
    }
    return s;
}

Solver::Solver(int s, std::string file, short instance, float ti, float tf, float cuto, int sizef, int fidiv):capacity(2)
{
    srand(s);
    max_time = INT_MAX;
    int num_nodes=0;
    int service_time=0;
    
    //standard values for SA
    temp_init = ti;
    temp_factor = tf;
    cutoff = cuto;
    size_factor = sizef;
    find_divisor = fidiv;
    
    //Read .vrs file
    ifstream f(file.c_str());
    while(!f.eof())
    {
        string varName;
        string varValue;
        string line;
        std::getline(f,line);
        int index = line.find(":");
        if(index!=string::npos)
        {
            varName = line.substr(0,index);
            varValue = line.substr(index+1,string::npos);
            trim(varValue);
        }
        else
        {
            varName=trim(line);
        }

        if(varName=="DIMENSION")
        {
            //Set Dimension
            num_nodes = std::stod(varValue);
            nodes.resize(num_nodes);
            nodes[0]=NULL;
        }
        else if(varName=="DISTANCE")
        {
            max_time = stoi(varValue);
        }
        else if(varName=="CAPACITY")
        {
            //Set Capacity;
            int c = stod(varValue);
            if(instance == 1){
                capacity[0] = c;
            } else {
                capacity[0] = 3*c/4.;
                capacity[1] = c/4.;
            }
            
        }
        else if(varName=="SERVICE_TIME")
        {
            service_time = stoi(varValue);
        }
        else if(varName=="VEHICLES")
        {
            //Set Vehicles
        }
        else if(varName=="NODE_COORD_SECTION")
        {
            //cout<<"COOOOOOOORDS"<<endl;
            //Set Nodes
            for(int i=0;i<num_nodes-1;i++)
            {
                int id;
                float x,y;
                node* n = new node();
                
                f>>id>>x>>y;
                
                n->demand.resize(2);
                n->node_id=i+1;
                n->time=service_time;
                n->x=x;
                n->y=y;
                nodes[id]=n;
            }
        }
        else if(varName=="DEMAND_SECTION")
        {
            //Set Demands
            for(int i=0;i<num_nodes-1;i++)
            {
                int id;
                int d;
                f>>id>>d;
                if(instance == 1){
                    nodes[id]->demand[0]=d;
                } else if(instance == 2){
                    if (nodes[id]->x > 0  &&
                        nodes[id]->x < 35 &&
                        nodes[id]->y > 0  &&
                        nodes[id]->y < 35){
                            //2:1
                            nodes[id]->demand[0] = 2*d/3.;
                            nodes[id]->demand[1] = d/3.;
                        
                    } else {
                        //3:1
                        nodes[id]->demand[0] = 3*d/4.;
                        nodes[id]->demand[1] = d/4.;
                    }
                } else if(instance == 3){
                    if (nodes[id]->x > 0  &&
                        nodes[id]->x < 35 &&
                        nodes[id]->y > 0  &&
                        nodes[id]->y < 35){
                            //2:1
                            nodes[id]->demand[0] = 2*d/3.;
                            nodes[id]->demand[1] = d/3.;
                        
                    } else {
                        //4:1
                        nodes[id]->demand[0] = 4*d/5.;
                        nodes[id]->demand[1] = d/5.;
                    }
                }
            }
            //cout<<"DEMANDDDDDDD"<<endl;
        }
        else if(varName=="DEPOT_SECTION")
        {
            //Set Depots
            float x,y;
            f>>x>>y;
            node *depot = new node();
            depot->node_id=0;
            depot->time=0;
            depot->x=x;
            depot->y=y;
            depot->demand.resize(2);
            depot->demand[0]=0;
            depot->demand[1]=0;
            nodes[0]=depot;
            //cout<<"DEPOTTTTTT"<<endl;
        }
    }
    
    neighborhood_size = 2*pow(nodes.size(),2);
}

Solution Solver::visitNeighbor(){
    int a,b,x;
    Solution temp(current);
    x = rand() % 3;
    a = rand() % current.getSize();
    b = rand() % current.getSize();
    if(x <= 1){
        temp.one2one(a,b);
    }else if(x <= 2){
        temp.deleteInsert(a, b);
    }else {
        temp.partialReverse(a, b);
    }
    return temp;
}

// SOURCE: https://en.wikipedia.org/wiki/Simulated_annealing
void Solver::start()
{
    best = Solution(nodes);
    init = best;
    current = best;
    float temp = temp_init;
    cout << "waiting for cooldown... \n";
    do
    {
        int trials=0;
        int changes=0;
        do
        {
            trials++;
            Solution n;
            n = visitNeighbor();
            if(n.isFeasable(max_time,capacity))
            {
                //cout << "found feasible solution, trials: " << trials << "\n";
                float energy_delta = n.evaluate()-current.evaluate();
                if(energy_delta<0.0)
                {
                    changes++;
                    current = n;
                    if(current.evaluate()<best.evaluate())
                    {
                        best=current;
                        //cout << " better solution found\n";
                    }
                }
                else
                {
                    if(exp(-energy_delta/temp)>(double(rand())/RAND_MAX))
                    {
                        changes++;
                        current = n;
                    }
                }
            }
        }while(trials<size_factor*neighborhood_size && changes < cutoff*neighborhood_size);
        temp = temp * temp_factor;
        cout << temp << "\n";
    }while(temp>(temp_init/find_divisor));
    cout << "Cold enough u can touch it! \n";
    printSolution();
}

void Solver::printSolution(){
    best.print();
}
Solution Solver::getSolution(){
    return best;
}
Solution Solver::getInit(){
    return init;
}
