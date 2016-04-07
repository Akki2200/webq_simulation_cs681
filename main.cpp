#include <iostream>
#include <queue>
#include <cfloat>
#include <climits>
using namespace std;

#define WAITING_TIME 5
#define SERVER_TIME 5

enum RequestType{
	USER,
	GENERATOR,
	WAITING,
	CHECKER,
	SERVER
};

class Request{

public:

	int id;					//unique id of the request
	float arrival_time;		//arrival time of the request
	float next_event_time;	//this request will occur at next_event_time
							//the request with the lowest next_event_time (most
							//  imminent) be processed first
	RequestType type;


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

	//not equalto
	friend bool operator!= (const Request& lhs, const Request& rhs){
		return lhs.next_event_time != rhs.next_event_time;
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

	priority_queue< Request, vector<Request>, greater<Request> > 
	                                  user, generator, waiting, checker, server;
	priority_queue< Request, vector<Request>, greater<Request> > events;

	Request max(INT_MAX, FLT_MAX);

	// create users
	for (int i = 100; i > 0; --i)
	{
		Request new_user(i, i/10.0f);
		user.push(new_user);
		events.push(new_user);
	}
	int line_number = 1;

	while(!events.empty()){
		Request next_event = events.top();
		events.pop();
		switch(next_event.type) {
			case USER:
				cout << "I am a user" << line_number++ << endl;
				cout << next_event << endl;
				if(user.top()!=next_event){
					cout << "Error processing USER event" << endl;
					cout << next_event << endl;
					return INT_MIN;
				}
				user.pop();
				next_event.type = GENERATOR;
				generator.push(next_event);
				events.push(next_event);
				break;

			case GENERATOR:
				cout << "I am a generator" << line_number++ << endl;
				cout << next_event << endl;
				if(generator.top()!=next_event){
					cout << "Error processing GENERATOR event" << endl;
					cout << next_event << endl;
					return INT_MIN;
				}
				generator.pop();
				next_event.type = WAITING;
				next_event.next_event_time += WAITING_TIME;
				waiting.push(next_event);
				events.push(next_event);
				break;

			case WAITING:
				cout << "I am a waiting" << line_number++ << endl;
				cout << next_event << endl;
				if(waiting.top()!=next_event){
					cout << "Error processing WAITING event" << endl;
					cout << next_event << endl;
					return INT_MIN;
				}
				waiting.pop();
				next_event.type = CHECKER;
				checker.push(next_event);
				events.push(next_event);
				break;

			case CHECKER:
				cout << "I am a checker" << line_number++ << endl;
				cout << next_event << endl;
				if(checker.top()!=next_event){
					cout << "Error processing CHECKER event" << endl;
					cout << next_event << endl;
					return INT_MIN;
				}
				checker.pop();
				next_event.type = SERVER;
				next_event.next_event_time += SERVER_TIME;
				server.push(next_event);
				events.push(next_event);
				break;

			case SERVER:
				cout << "I am a server" << line_number++ << endl;
				cout << next_event << endl;
				if(server.top()!=next_event){
					cout << "Error processing SERVER event" << endl;
					cout << next_event << endl;
					return INT_MIN;
				}
				server.pop();
				break;

			default:
				cout << "Error processing event's RequestType" << endl;
				return INT_MIN;
		}
	}

	//print the queue
	for (int i = 0; i < 100 ; ++i)
	{
		// Request r1 = genera.front(); generator.pop();
		// Request r2 = generator.front(); generator.pop();
		// cout<< (r1 < r2) <<endl;

		// cout << user.front() << endl;
		// Request new_client(generator.top().id,generator.top().arrival_time);
		// waiting_room_q.push(new_client);
		// user.pop();
	}	
	return 0;
}
