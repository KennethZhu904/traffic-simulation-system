#include "Platoon.h"
#include <string>
#include <iostream>
using namespace std;

Platoon::Platoon(string init)
{
    // the string 'init' looks like:
    // a,b ; c,d ; ... where, a and c are the vehicle id, b and d are positions.
    stringstream ss(init);
    string segment;
    while(getline(ss, segment, ';'))
    {
        size_t found = segment.find(",");
        if (found!=std::string::npos){
            string id = segment.substr(0, found);
            string pos = segment.substr(found+1, segment.size()-1);

            // conversion to integer
            stringstream s1(id);
            int id_value;
            s1 >> id_value;
            stringstream s2(pos);
            int pos_value;
            s2 >> pos_value;
            Car* car = new Car(id_value, pos_value);

            if (head == NULL)
            {
                head = car;
                tail = car;
            }
            else
            {
                tail->set_next(car);
                car->set_prev(tail);
                tail = car;
                car->set_next(NULL);
            }
        }
    }
}

Car* Platoon::get_tail()
{
    return tail;
}

Car* Platoon::get_head()
{
    return head;
}

// Get the car that is less than the car is going to be inserted in the array.
//This is recursion function
bool  Platoon:: check_clashes(Car* c, int position){

    if (c->get_position()==position){

        return true;

    } else if (c->get_prev()==NULL) {

        return false;


    } else {

        check_clashes(c->get_prev(), position);
    }

}



void Platoon::remove(Car* c)
{

    bool both_headandtail=isCar_both_head_and_tail(c);

    //This condition check if the car removed is both head and tail (i.e. only car in the lane)

    if (!both_headandtail){
        //Checking if the car removed is HEAD
        if (c==head){

            Car* temp_forward= c->get_next();

            //Set the car in front of removed to NULL for previous
            temp_forward->set_prev(NULL);
            c->set_next(NULL);
            head= temp_forward;

        }else if (c==tail){

            Car* temp_previous= c->get_prev();

            temp_previous->set_next(NULL);
            c->set_prev(NULL);
            c->set_next(NULL);
            tail=temp_previous;

        } else{

            Car* temp_forward= c->get_next();
            Car* temp_previous= c->get_prev();


            //Set that car in front of the removed car to the car that is before removed car
            temp_forward->set_prev(temp_previous);

            //Set the car behind the removed car to the car in front of the removed car
            temp_previous->set_next(temp_forward);

            //Set the removed car to have null for set next and set_backwards

            c->set_next(NULL);
            c->set_prev(NULL);

        }
    } else {


        head=NULL;
        tail=NULL;

    }

}


void Platoon::append(Car* c){

    Car*node= get_tail();
    if (tail==NULL){

        head=c;
        tail=c;

    } else {

        c->set_prev(node);
        node->set_next(c);
        tail= c;

    }

}

void Platoon:: prepend(Car* c){

    Car*node= get_head();

    if (head==NULL){

        head=c;
        tail=c;

    } else {

        c->set_next(node);
        node->set_prev(c);
        head= c;

    }

}


void Platoon:: insert (Car* c){


    Car* car_tail=tail;
    Car* car_head=head;


    // Checking if there are any cars in the next lane. If there is no cars, use prepend function.
    if (check_no_cars_innext_lane(tail)){

        prepend(c);

        //Checking if there is only one car in the lane. To do this , the head and tail must be equal to each other.
    } else if (head==tail ) {

        int head_position= car_head->get_position();
        if (c->get_position()>head_position){

            append(c);

        } else{

            prepend(c);
        }
    }
    //Checking if the car inserted is going to be above the tail
    //If this is the case, it will append.
    else if (c->get_position()>car_tail->get_position()){
        append(c);
        //Checking if the car inserted will be less than the head
    } else if (c->get_position()< car_head->get_position()){
        prepend(c);

    } else {
        //Condition for putting between cars.
        Car* front_Car=get_car_front(head,c->get_position());
        Car* previous_car=get_prev_car(tail,c->get_position());

        c->set_next(front_Car);
        front_Car->set_prev(c);

        previous_car->set_next(c);
        c->set_prev(previous_car);
    }
}


bool Platoon:: check_no_cars_innext_lane(Car* c){

    if (tail==NULL && head==NULL){

        return true;

    } else {

        return false;
    }


}


Car* Platoon:: get_prev_car(Car* c, int position){

    if(c->get_position()< position){

        return c;
    } else {


        return get_prev_car(c->get_prev(),position);
    }
}

Car* Platoon:: get_car_front(Car* c, int position){

    if(c->get_position()> position){

        return c;
    } else {


        return get_car_front(c->get_next(),position);
    }
}


//This function will flag if the car inserted or removed is a tail and/or head.
bool Platoon:: flag_raised(Car* c){
    if (!isCar_both_head_and_tail(c)){

        if (c==tail ){

            return true;

        } else {

            return false;
        }

    } else{

        return true;

    }
}

bool Platoon:: isCar_both_head_and_tail(Car* c){

    if (c==head && c==tail){

        return true;
    } else{

        return false;
    }

}














