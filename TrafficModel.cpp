#include "TrafficModel.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

TrafficModel::TrafficModel() { }
TrafficModel::~TrafficModel(){ }

void TrafficModel::set_commands(vector<string> commands)
{
    this->commands = commands;
}

/* A helper function.
 * Given a specific id as the input, it searches the lane change command stored in
 * "commands". Returns the value of the turn-light in the command for this car id.
 * 0 = no light, 1 = left light, 2 = right light.
 */
int TrafficModel::get_lane_change_command(int id)
{
    int count = commands.size();
    for (int i = 0; i < count; i++){
        size_t found = commands[i].find(',');
        string iter_id = commands[i].substr(0, found);
        string light = commands[i].substr(found+1, commands[i].size()-1);
        stringstream ss1(iter_id);
        int id_value;
        ss1 >> id_value;
        if (id_value == id){
            stringstream ss2(light);
            int light_value;
            ss2 >> light_value;
            return light_value;
        }
    }
    return 0;
}

/*
 * The function that updates the vehicle positions and states.
 */
void TrafficModel::update()
{

    std::vector<int>listofids;
    bool lane_free_check;

    // The for loop cycles through the each platoons
    for (unsigned int i=0; i< platoons.size(); i++){
        Car* tail= platoons[i].get_tail();

        if (tail!=NULL){

            Car* random=tail;
            while (random!=NULL){
                int id=random->get_id();
                bool random_change = false;
                bool flag_raised=false;

                /* This condition checks to see if ID has already moved and stored in the array.
                 * If there is no ID stored in the array already, then it will perform.
                 */
                if (!check_ID_in_id(listofids,id)){

                    // Checks if the car is going straight
                    if (get_lane_change_command(id)==0){
                        move_forward(random);
                        //Similarly if the car is turning right
                    } else if (get_lane_change_command(id)==2){

                        lane_free_check=avaiability_check(2, i,platoons.size(), random->get_position());

                        if (lane_free_check){

                            flag_raised=platoons[i].flag_raised(random);

                            platoons[i].remove(random);
                            platoons[i+1].insert(random);

                            // If there is a tail left after removing from current platoons, then get the tail and start
                            // from there.
                            if (platoons[i].get_tail()!=NULL){
                                random=platoons[i].get_tail();
                            }


                        } else{

                            move_forward(random);
                        }
                        // Else condition is for left command.
                    } else {

                        lane_free_check=avaiability_check(1, i,platoons.size(), random->get_position());
                        // If the lane is free, then it will perform the left, otherwise more forward.
                        if (lane_free_check){

                            flag_raised=platoons[i].flag_raised(random);

                            platoons[i].remove(random);
                            platoons[i-1].insert(random);

                            if (platoons[i].get_tail()!=NULL){
                                random=platoons[i].get_tail();
                            }

                        } else {

                            move_forward(random);

                        }

                    }

                    random = random->get_prev();


                    //Verify that variable has changed
                    random_change = true;

                    // Add the id that has already commuted to the array
                    listofids.push_back(id);

                }


                //If the flag has not raised, then it will perform the previous function.
                //Otherwise, it will go and start running from the tail.
                if (flag_raised==false){

                    if (!random_change){

                        random = random->get_prev();
                    }
                } else {

                    random=platoons[i].get_tail();

                }

            }
        }
    }


}

/*
 * Initialization based on the input information
 */
void TrafficModel::initialize(vector<string> info)
{
    int lane_count = info.size();
    for (int i = 0; i < lane_count; i++){
        Platoon p = Platoon(info[i]);
        platoons.push_back(p);
    }
}

// Returns all the vehicle states in the system
vector<string> TrafficModel::get_system_state()
{
    vector<string> output;
    int size = platoons.size();
    for (int i = 0; i < size; i++){
        // get the last vehicle in the platoon
        Car* temp = platoons[i].get_tail();
        string s = "";
        ostringstream out;
        while (temp != NULL){
            out << ";(" << temp->get_id() << "," << i << "," << temp->get_position() << \
                    "," << get_lane_change_command(temp->get_id()) << ")";
            temp = temp->get_prev();
        }

        output.push_back(out.str());
    }
    return output;
}



bool TrafficModel:: avaiability_check(int command, int platoonlocation, int platoon_size, int car_position){


    bool clashes=false;

    //This condition checks the turning command (2=Right, 1=left)
    if (command==2){

        int nextplatoon=platoonlocation+1;
        bool in_range=within_the_range_check(nextplatoon, platoon_size);

        /* Checking if the car is not going out of the box. In other words,
         * not going out from the top or from the bottom.
         */

        if (!in_range){

            return false;
        }

        //If tail pointer is null in the next platoon, there is no need to check for clashes
        if ( platoons[nextplatoon].get_tail()!=NULL){
            clashes= platoons[nextplatoon].check_clashes(platoons[nextplatoon].get_tail(), car_position);
        }

        if (clashes){

            return false;

        } else if (in_range){

            return true;
        } else{
            //Returns false if it is outside of the range.
            return false;
        }


    } else {

        int nextplatoon=platoonlocation-1;
        bool in_range=within_the_range_check(nextplatoon, platoon_size);

        //Check if it is going outside of the box or not
        if (!in_range){


            return false;
        }




        //If tail pointer is null in the next platoon, there is no need to check for clashes
        if ( platoons[nextplatoon].get_tail()!=NULL){
            clashes= platoons[nextplatoon].check_clashes(platoons[nextplatoon].get_tail(), car_position);
        }
        // Check if there are any clashes and also checking if thye are in the car's range.
        if (clashes){
            return false;

        } else if (in_range){

            return true;
        } else{

            return false;
        }
    }

}


bool TrafficModel:: within_the_range_check(int nextplatoon, int platoon_size){


    if (nextplatoon==platoon_size){
        return false;
    } else if (nextplatoon==-1){
        return false;
    } else{

        return true;
    }

}

void TrafficModel:: move_forward(Car* random){

    // This conditions= checks if there are are cars occupied in front of propsed wave move.
    if (check_move_forward(random)){

        int currentposition=random->get_position();
        random->set_position(currentposition+1);
    }

}

//Check to see if the car can be moved forward
bool TrafficModel::check_move_forward(Car* random){

    Car* front_car=random->get_next();
    int proposed_new_position= random->get_position()+1;


    if (front_car==NULL){

        return true;
    } else if (front_car->get_position()==proposed_new_position){

        return false;

    } else {

        return true;
    }

}

//Checks to see if there is ID already in the array.
bool TrafficModel:: check_ID_in_id(vector<int> list, int id_checked){

    for (unsigned int i=0; i<list.size(); i++){

        if (id_checked==list[i]){

            return true;
        }
    }

    return false;

}















