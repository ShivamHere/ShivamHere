#include <bits/stdc++.h>
using namespace std;

#define AMT 20;
enum RideStatus{IDLE, CREATED, WITHDRAWN, COMPLETED};

class Ride
{
    int id;
    int origin, dest;
    int seats;
    RideStatus ridestatus;
public:
    static const int AMT_PER_KM = AMT;
    Ride();
    int calculateFare(bool);
    void setId(int);
    void setOrigin(int);
    void setDest(int);
    void setSeats(int);
    void setRideStatus(RideStatus);
    int getId() const;
    RideStatus getRideStatus() const;
};

class Person
{
public:
    string name;
};

class Rider : private Person
{
    vector<Ride> completedRides;
    vector<Ride> goingRides;

public:
    Rider(string);
    void createRide(int,int,int,int);
    void updateRide(int,int,int,int);
    void withdrawRide(int);
    int closeRide(int);
};

class Driver : private Person
{
public:
    Driver(string);
};

Rider :: Rider(string name)
{
    this->name = name;
}

Driver :: Driver(string name)
{
    this->name = name;
}

Ride::Ride()
{
    origin = dest = seats = id = 0;
    ridestatus = RideStatus::IDLE;
}
void Ride :: setId(int id)
{
    this->id = id;
}
void Ride :: setOrigin(int origin)
{
    this->origin = origin;
}
void Ride :: setDest(int dest)
{
    this->dest = dest;
}
void Ride :: setSeats(int seats)
{
    this->seats = seats;
}
void Ride :: setRideStatus(RideStatus ridestatus)
{
    this->ridestatus = ridestatus;
}
int Ride :: getId() const
{
    return id;
}
RideStatus Ride :: getRideStatus() const
{
    return ridestatus;
}
int Ride :: calculateFare(bool isPriorityRider)
{
    int dist = dest-origin;
    if(seats<2) return dist*AMT_PER_KM*(isPriorityRider?0.75:1);
    return seats*dist*AMT_PER_KM*(isPriorityRider?0.5:0.75);
}
void Rider::createRide(int id, int origin, int dest, int seats)
{
    if(origin >= dest)
    {
        cout<<"Wrong values provided. Can't create ride"<<endl;
        return;
    }
    Ride currentRide;
    currentRide.setId(id);
    currentRide.setOrigin(origin);
    currentRide.setDest(dest);
    currentRide.setSeats(seats);
    currentRide.setRideStatus(RideStatus::CREATED);
    goingRides.push_back(currentRide);
}

void Rider::updateRide(int id, int origin, int dest, int seats)
{
    auto itr = goingRides.begin();
    for(;itr != goingRides.end(); itr++){
        if(itr->getId() == id)  break;
    }
    if(itr->getRideStatus() != RideStatus::CREATED)
    {
        cout<<"Can't update. Ride was not in progress"<<endl;
        return;
    }
    itr->setOrigin(origin);
    itr->setDest(dest);
    itr->setSeats(seats);

}

void Rider::withdrawRide(int id)
{
    auto itr = goingRides.begin();
    for(;itr != goingRides.end(); itr++){
        if(itr->getId() == id)  break;
    }
    if(itr->getRideStatus() != RideStatus::CREATED)
    {
        cout<<"Can't withdraw. Ride was not in progress"<<endl;
        return;
    }
    itr->setRideStatus(RideStatus::WITHDRAWN);
}

int Rider::closeRide(int id)
{
    auto itr = goingRides.begin();
    for(;itr != goingRides.end(); itr++){
        if(itr->getId() == id)  break;
    }
    if(itr->getRideStatus() != RideStatus::CREATED)
    {
        cout<<"Can't close. Ride was not in progress"<<endl;
        return 0;
    }
    itr->setRideStatus(RideStatus::COMPLETED);
    completedRides.push_back(*itr);
    int fee =  itr->calculateFare(completedRides.size()>=10);
    goingRides.erase(itr);
    return fee;
}

int main(){
    Rider r("Kapil");
    Driver d("Dev");
    r.createRide(1,50,60,1);
    cout<<r.closeRide(1)<<endl;
    r.updateRide(1,50,60,2);
    cout<<r.closeRide(1)<<endl;
    cout<<"----------------------------------------"<<endl;
    r.createRide(1,50,60,1);
    r.withdrawRide(1);
    r.updateRide(1,50,60,2);
    cout<<r.closeRide(1)<<endl;
    return 0;
}
