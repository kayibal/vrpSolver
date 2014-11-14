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

Solver::Solver(int s, int max_t, int c, std::string file)
{
    srand(s);
    max_time = max_t;
    int num_nodes=0;

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
        else if(varName=="CAPACITY")
        {
            //Set Capacity;
            capacity = stod(varValue);
        }
        else if(varName=="VEHICLES")
        {
            //Set Vehicles
        }
        else if(varName=="NODE_COORD_SECTION")
        {
            cout<<"COOOOOOOORDS"<<endl;
            //Set Nodes
            for(int i=0;i<num_nodes-1;i++)
            {
                int id;
                float x,y;
                node* n = new node();

                f>>id>>x>>y;

                n->node_id=i+1;
                n->time=0;
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
                nodes[id]->demand=d;
            }
            cout<<"DEMANDDDDDDD"<<endl;
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
            depot->demand=0;
            nodes[0]=depot;
            cout<<"DEPOTTTTTT"<<endl;
        }
    }
}


