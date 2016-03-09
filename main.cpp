
#include <iostream>
#include <queue>
using namespace std;


enum RequestType{
	USER,
	GENERATOR,
	WAITING,
	CHECKER,
	SERVER
};

class Request{
	int id;					//unique id of the request
	float arrival_time;		//arrival time of the request
	float next_event_time;	//this request will occur at next_event_time
							//the request with the lowest next_event_time (most
							//  imminent) be processed first
	RequestType type;

public:
	Request(int id, float arrival_time){
		this->id = id;
		this->arrival_time = arrival_time;
		this->next_event_time = arrival_time;
		this->type = USER;
	}

	//less than
	friend bool operator< (const Request& lhs, const Request& rhs){
		return lhs.next_event_time < rhs.next_event_time;
	}

	//greater than
	friend bool operator> (const Request& lhs, const Request& rhs){
		return lhs.next_event_time > rhs.next_event_time;
	}

	//print request description
	friend ostream& operator<< (ostream &output, const Request &r ){
		output << "Request [ID: " << r.id << ", arrival_time: "
		 << r.arrival_time << ", next_event_time: " << r.next_event_time
		 << ", type: " << r.type << "]";
		return output;            
	}

};


int main(int argc,char** argv){


	priority_queue< Request, vector<Request>, greater<Request> > clients;

	//fill the queue
	for (int i = 100; i > 0; --i)
	{
		Request new_client(i, (float)i/10);
		clients.push(new_client);
	}

	//print the queue
	for (int i = 0; i < 100 ; ++i)
	{
		// Request r1 = clients.front(); clients.pop();
		// Request r2 = clients.front(); clients.pop();
		// cout<< (r1 < r2) <<endl;
		cout << clients.top() << endl;
		clients.pop();
	}	
	return 0;
}
