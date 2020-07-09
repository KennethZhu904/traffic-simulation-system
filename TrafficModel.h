#ifndef _TrafficModel_H_
#define _TrafficModel_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <string>
using namespace std;
#include "Platoon.h"

#include "Car.h"

using namespace std;

class TrafficModel
{
    private:
        vector<Platoon> platoons;
        vector<string> commands; // stores a set of lane change commands

    public:
        TrafficModel();
        ~TrafficModel();

        int get_lane_change_command(int id);
        void set_commands(vector<string> commands);
        void initialize(vector<string> info);
        void update();
        bool avaiability_check(int command,int platoonlocation,int platoon_size,int carposition);
        bool within_the_range_check(int nextplatoon, int platoon_size);
        void move_forward(Car* random);
        bool check_move_forward(Car* random);
        bool check_ID_in_id(vector<int> list, int id_checked);
        bool check_tail(Car* c, Car* tail);
        vector<string> get_system_state();
};


#endif







