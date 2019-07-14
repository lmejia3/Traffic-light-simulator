// Copy and paste your work, or start typing.

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

using namespace std;

/*This input file will look like:
E // Initial direction
1 // Max number of cars per direction
ABC N 2 // Car info (license plate, direction, time)
BCD N 2
CDE S 3
DEF E 2
EFG S 1
FGH W 1*/

class Process {
	int time, numOfCars;
	char direction;
	string license;

public:
	Process() {}
	~Process() {}
	Process(char direction, string license, int time, int numOfCars) {
		this->direction = direction;
		this->license = license;
		this->time = time;
		this->numOfCars = numOfCars;
	}


	Process(const Process &s) {
		direction = s.direction;
		license = s.license;
		time = s.time;
		numOfCars = s.numOfCars;
	}

	char getdirection() { return direction; }
	string getlicense() { return license; }
	int gettime() { return time; }
	int getnumOfCars() {return numOfCars; }

    string getDirectionDescription(){

        string description = "";
        switch(direction){
            case 'N': description = "Northbound"; break;
            case 'E': description = "Eastbound"; break;
            case 'S': description = "Southbound"; break;
            case 'W': description = "Westbound"; break;

        }

        return description;
    }

	friend ostream& operator<<(ostream &out, const Process &pr) {
		out << "Car " << pr.license << " is using intersection for "
			<< pr.time << " sec(s).";
		return out;
	}

};

char getNextDirection(char direction){
    char next = ' ';

    switch(direction) {
        case 'N': next = 'E'; break;
        case 'E': next = 'S'; break;
        case 'S': next = 'W'; break;
        case 'W': next = 'N'; break;
    }

    return next;
}

int main() {


    //std::ifstream in("hw1.txt");
    //std::streambuf *cinbuf = std::cin.rdbuf(); //save old buf
    //std::cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!

	vector<Process> temp; //temp vector
    vector<Process> north;
    vector<Process> east;
    vector<Process> south;
    vector<Process> west;

	vector<Process> v;
	int max = 0, time = 0;
	string license = "";
	char direction = ' ';
    char initialDirection = ' ';

	cin >> initialDirection;
	cin >> max;

	while(cin >> license >>direction>> time) {

        Process p(direction, license, time, max);

        switch(direction) {
          case 'N' : north.insert(north.begin(), p); break;
          case 'E' : east.insert(east.begin(), p); break;
          case 'S' : south.insert(south.begin(), p); break;
          case 'W' : west.insert(west.begin(), p); break;
        }
	}

  	char currentDirection = initialDirection;
        int cars = max;

  	while(1){
     	vector<Process> *temp = nullptr;

      	switch(currentDirection) {
              case 'N' :
            	temp = &north; break;
              case 'E' :
            	temp = &east; break;
              case 'S' :
            	temp = &south; break;
              case 'W' :
            	temp = &west; break;
        }

        if(temp->size() > 0){
          v.insert(v.begin(), temp->back());
          temp->pop_back();
        }

        cars--;

        if(cars == 0){
            currentDirection = getNextDirection(currentDirection);
        }

        if(cars < 1){
            cars = max;
        }

        //all queues emptied out... so break out of loop
        if(north.size() == 0 && east.size() == 0 && south.size() == 0 && west.size() == 0){
          break;
        }
    }

    /* debug for looping through vector
    int i=0;
    vector<Process>::iterator it;  // declare an iterator to a vector of strings
    for(it = v.begin(); it != v.end(); it++,i++ ) {

        cout<< *it << endl;  // prints d.
    }
    */

    while(v.size() > 0){

        int id = fork();
        Process p = v.back();
        v.pop_back();

        //if child process
        if(id == 0){
            cout << "Current Direction: " << p.getDirectionDescription() << endl;
            cout << p << endl;
            exit(0);
        }
        else{ //assume we are in parent
            //parent sleeps  based on process time
            sleep(p.gettime());
        }
    }

    return 0;
}

