#include <bits/stdc++.h>
using namespace std;

#define AMT 20;
enum RideStatus {IDLE, CREATED, WITHDRAWN, COMPLETED};

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

class Driver: private Person
{
public:
    Driver(string);
};

class Rider: private Person
{
    int id;
    vector<Ride> completedRides;
    Ride currentRide;
public:
    Rider(int, string);
    void createRide(int, int, int, int);
    void updateRide(int, int, int, int);
    void withdrawRide(int);
    int closeRide();
    int getRiderId();
};

class System {
    int drivers;
    vector<Rider> riders;
public:
    System(int, vector<Rider>&);
    void createRide(int, int, int, int, int);
    void updateRide(int, int, int, int, int);
    void withdrawRide(int, int);
    int closeRide(int);
};

Ride::Ride()
{
    id = origin = dest = seats = 0;
    ridestatus = RideStatus::IDLE;
}

System::System(int drivers, vector<Rider>& riders)
{
    this->drivers = drivers;
    this->riders = riders;
}

int Ride::calculateFare(bool isPriorityRider)
{
    int dist = dest - origin;
    if(seats<2) return dist*AMT_PER_KM*(isPriorityRider?0.75:1);
    return seats*dist*AMT_PER_KM*(isPriorityRider?0.5:0.75);
}

Driver::Driver(string name)
{
    this->name = name;
}

void Ride::setId(int id){
    this->id = id;
}
void Ride::setOrigin(int origin){
    this->origin = origin;
}
void Ride::setDest(int dest){
    this->dest = dest;
}
void Ride::setSeats(int seats){
    this->seats = seats;
}
void Ride::setRideStatus(RideStatus ridestatus){
    this->ridestatus = ridestatus;
}
int Ride::getId() const{
    return id;
}
RideStatus Ride::getRideStatus() const{
    return ridestatus;
}

Rider::Rider(int id, string name)
{
    this->id = id;
    this->name = name;
}

void Rider::createRide(int id, int origin, int dest, int seats)
{
    if(origin >= dest)
    {
        cout<<"Wrong values provided. Can't create ride"<<endl;
        return;
    }
    currentRide.setId(id);
    currentRide.setOrigin(origin);
    currentRide.setDest(dest);
    currentRide.setSeats(seats);
    currentRide.setRideStatus(RideStatus::CREATED);
}

void Rider::updateRide(int id, int origin, int dest, int seats)
{
    if(currentRide.getRideStatus() == RideStatus::WITHDRAWN)
    {
        cout<<"Can't update ride. Ride was withdrawn."<<endl;
        return;
    }
    if(currentRide.getRideStatus() == RideStatus::COMPLETED)
    {
        cout<<"Can't update ride. Ride was already completed."<<endl;
        return;
    }
    createRide(id,origin,dest,seats);
}

void Rider::withdrawRide(int id)
{
    if(currentRide.getId() != id)
    {
        cout<<"Wrong ride Id as input. Can't withdraw current ride.\n";
        return;
    }
    if(currentRide.getRideStatus() != RideStatus::CREATED)
    {
        cout<<"Ride wasn't in progress. Can't withdraw ride"<<endl;
        return;
    }
    currentRide.setRideStatus(RideStatus::WITHDRAWN);
}

int Rider::closeRide()
{
    if(currentRide.getRideStatus() != RideStatus::CREATED)
    {
        cout<<"Ride wasn't in progress. Can't close ride"<<endl;
        return 0;
    }
    currentRide.setRideStatus(RideStatus::COMPLETED);
    completedRides.push_back(currentRide);
    return currentRide.calculateFare(completedRides.size()>=10);
}

void System::createRide(int riderid, int rideid, int origin, int dest, int seats)
{
    if(drivers == 0)
    {
        cout<<"Can't create a ride. No driver available.\n";
        return;
    }
    for(Rider &rider : riders)
    {
        if(rider.getRiderId()==riderid)
        {
            rider.createRide(rideid, origin, dest, seats);
            drivers--;
            break;
        }
    }
}

void System::updateRide(int riderid, int rideid, int origin, int dest, int seats)
{
    for(Rider &rider : riders)
    {
        if(rider.getRiderId()==riderid)
        {
            rider.updateRide(rideid, origin, dest, seats);
            break;
        }
    }
}

void System::withdrawRide(int riderid, int rideid)
{
    for(Rider &rider : riders)
    {
        if(rider.getRiderId()==riderid)
        {
            rider.withdrawRide(rideid);
            drivers++;
            break;
        }
    }
}

int System::closeRide(int riderid)
{
    for(Rider &rider : riders)
    {
        if(rider.getRiderId()==riderid)
        {
            drivers++;
            return rider.closeRide();
        }
    }
    return 0;
}

int main(){
    return 0;
}
