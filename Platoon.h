#ifndef _Platoon_H_
#define _Platoon_H_

#include <vector>
#include <string>
#include <sstream>
#include "Car.h"

using namespace std;

class Platoon
{
    private:
        Car* head = NULL;
        Car* tail = NULL;

    public:
        Platoon(string init);
        Car* get_tail();
        Car* get_head();
        void remove(Car* c);
        void append(Car* c);
        void prepend(Car* c);
        void insert (Car* c);
        bool check_clashes(Car* c, int position);
        Car* get_prev_car(Car* c, int position);
        Car* get_car_front(Car* c, int position);
        bool check_no_cars_innext_lane (Car* c);
        bool isCar_both_head_and_tail(Car* c);
        bool flag_raised(Car* c);


};
#endif









