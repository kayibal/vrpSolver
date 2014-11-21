#include<fstream>
#include<string>
#include<climits>
#include<cmath>
using namespace std;

struct point
{
    float x,y;
};

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


int main(int argc,char**argv)
{
    string name;
    string type;
    string comment;
    if(argc<1)
    {
        printf("Usage: translate <.vrp>");
        return -1;
    }
    string file_name(argv[1]);
    //Read .vrs file
    ifstream f(file_name.c_str());
    float* distance_matrix;
    point* cities;
    float* demands;
    float* service_times;
    int max_time=INT_MAX;
    float capacity=0;
    int service_time=0;
    int num_cities;
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
            demands = new float[stoi(varValue)];
            cities = new point[stoi(varValue)];
            service_times = new float[stoi(varValue)];
            distance_matrix = new float[stoi(varValue)*stoi(varValue)];
            num_cities = stoi(varValue);
        }
        else if(varName=="DISTANCE")
        {
            max_time = stod(varValue);
        }
        else if(varName=="CAPACITY")
        {
            //Set Capacity;
            capacity = stod(varValue);
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
            //Set Nodes
            for(int i=0;i<num_cities-1;i++)
            {
                int id;
                float x,y;

                f>>id>>x>>y;

                cities[id].x=x;
                cities[id].y=y;
                service_times[id]=service_time;
            }
        }
        else if(varName=="DEMAND_SECTION")
        {
            //Set Demands
            for(int i=0;i<num_cities-1;i++)
            {
                int id;
                int d;
                f>>id>>d;
                demands[id]=d;
            }
        }
        else if(varName=="DEPOT_SECTION")
        {
            //Set Depots
            float x,y;
            f>>x>>y;
            cities[0].x=x;
            cities[0].y=y;
            service_times[0]=0;
            demands[0]=0;
        }
    }
    //Calculate distance Matrix
    for(int y=0;y<num_cities;y++)
    {
        for(int x=0;x<num_cities;x++)
        {
            distance_matrix[x+y*num_cities] = sqrt(pow((cities[x].x-cities[y].x),2)+pow(cities[x].y-cities[y].y,2));
        }
    }
    f.close();
    ofstream o(file_name.replace(file_name.end()-3,file_name.end(),"dat").c_str());
    o<<"data;"<<endl;
    o<<"param n := "<<num_cities<<endl;
    o<<"param Dmax := "<<max_time<<endl;
    o<<"param Cmax := "<<capacity<<endl;

    //Set demans
    o<<"param : V :  c :="<<endl;
    for(int i=0;i<num_cities;i++)
    {
        o<<i<<" "<<demands[i]<<endl;
    }

    //Set Distances
    o<<"param : E : d :="<<endl;
    for(int y=0;y<num_cities;y++)
    {
        for(int x=0;x<num_cities;x++)
        {
            o<<y<<" "<<x<<" "<<distance_matrix[x+y*num_cities]<<endl;
        }
    }
    o<<";"<<endl;

    //Set Servie Time
    o<<"param : V : t :="<<endl;
    for(int i=0;i<num_cities;i++)
    {
         o<<i<<" "<<service_times[i]<<endl;
    }
    o<<";"<<endl;

    o<<"end;"<<endl;
    o.close();
    return 0;
}
