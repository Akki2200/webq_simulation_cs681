#include <iostream>
#include <queue>
#include <cfloat>
#include <climits>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include <tr1/random>
#include "random_generator.hpp"
// #include "lcgrand.h"

using namespace std;

#define WAITING_RATE 5.0f
#define GENERATOR_RATE 1.0f
#define CHECKER_RATE 5.0f
#define SERVER_RATE 5.0f
float global_clock = 0.0f;

enum RequestType{
	DONE,
	USER_DEQ,
	GENERATOR_ENQ,
	GENERATOR_DEQ,
	WAITING_ENQ,
	WAITING_DEQ,
	CHECKER_ENQ,
	CHECKER_DEQ,
	SERVER_ENQ,
	SERVER_DEQ,
	DROP
};


class Request{

public:

	int id;					//unique id of the request
	float arrival_time;		//arrival time of the request
	float next_event_time;	//this request will occur at next_event_time
							//the request with the lowest next_event_time (most
							//  imminent) be processed first
	RequestType type;
	float user_deq, generator_enq, generator_deq, waiting_enq, waiting_deq,
	      checker_enq, checker_deq, server_enq, server_deq;


	Request(int id, float arrival_time){
		this->id = id;
		this->arrival_time = arrival_time;
		this->next_event_time = arrival_time;
		this->type = USER_DEQ;
		user_deq = generator_enq = generator_deq = waiting_enq = waiting_deq =
	      checker_enq = checker_deq = server_enq = server_deq = -1.0f/0.0f;


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
		output << "Request [ ID: " << r.id << " arrival_time: "
		 << r.arrival_time << " next_event_time: " << r.next_event_time
		 << " type: " << r.type << " ]";
		return output;            
	}

	string show_times(){
		ostringstream times;
		times << "[ " << user_deq << " " << generator_enq <<" "<< generator_deq << " " << waiting_enq << " " << waiting_deq << " " <<
	      checker_enq << " " << checker_deq << " " << server_enq << " " << server_deq << " ]";
	    return times.str();
	}

};


void print_the_queue(priority_queue< Request, vector<Request>, greater<Request> > pq){
	cout << "Printing queue..." << endl;
	while(!pq.empty()){
		cout << "\t" << pq.top() << endl;
		pq.pop();
	}
	cout << endl << endl;
}

int main(int argc,char** argv){

	// priority_queue< Request, vector<Request>, greater<Request> > 
	                               // user, generator, waiting, checker, server;
	queue< Request > user, generator, waiting, checker, server, done, drop;

	priority_queue< Request, vector<Request>, greater<Request> > events;

	int generator_completed = 0, checker_completed = 0, server_completed = 0;

	Request max(INT_MAX, FLT_MAX);
	float service_time_generator, service_time_checker, service_time_server,
	                                                               burst_period;
	int generator_queue_size, checker_queue_size, server_queue_size;
	int i = 0;
	float simulation_time = 0.0f , simulation_end_time = 1.0f/0.0f;
	cin >> simulation_end_time;
	cin >> service_time_generator >> service_time_checker;
	cin >> service_time_server >> burst_period;
	cin >> generator_queue_size >> checker_queue_size >> server_queue_size;
	
	// Random number generator engines. Initialise them with seeds.
	tr1::mt19937 user_eng(1234), generator_eng(1234),
	                                        checker_eng(1234), server_eng(1234);
	
	tr1::uniform_real<float> user_distr(0, burst_period);
	// create users
	while(simulation_time < simulation_end_time){
		// i++;
		// if(i>1000)
		// {
		// 	cout << "Shit Happened";
		// 	return 0;
		// }
		float peak_time= simulation_time + fmod(rand(),burst_period);//user_distr(user_eng);
		float trough_time= simulation_time + peak_time + fmod(rand(),burst_period);// user_distr(user_eng);

	// scanf("%f %f",&peak_time,&trough_time);

		while(simulation_time < trough_time){
			simulation_time += generate_next_event_time(peak_time,trough_time,simulation_time);
			// cout<<simulation_time<<endl;
			Request new_user(++i, simulation_time);
			user.push(new_user);
			events.push(new_user);
		}
	}
	int line_number = 1;
	float waiting_estimated_waiting_time = 0.0f, generator_service_time = 0.0f, 
						checker_service_time = 0.0f, server_service_time = 0.0f;

	// priority_queue< Request, vector<Request>, greater<Request> > to_print;
	queue < Request > to_print;
	while(!events.empty()){
		Request next_event = events.top();
		simulation_time = next_event.next_event_time;
		events.pop();
		switch(next_event.type) {
			case USER_DEQ:
				cout << line_number++ << "§ I am a dequeue user." << endl;
				cout << "\t" << next_event << endl;
				cout << "\t" << "Global Clock Time " << simulation_time << endl;
				cout << "-------------------------------------------------------" << endl<< endl;
				next_event.user_deq = simulation_time;
				next_event.type = GENERATOR_ENQ;
				user.pop();
				events.push(next_event);
				// to_print = events;
				// print_the_queue(to_print);
				break;

			case GENERATOR_ENQ:
				cout << line_number++ << "§ I am a enqueue generator." << endl;
				cout << "\t" << next_event << endl;
				cout << "\t" << "Global Clock Time " << simulation_time << endl;
				cout << "-------------------------------------------------------" << endl<< endl;
				next_event.generator_enq = simulation_time;
				if(generator.size() >= generator_queue_size) {	
					// next_event.type = DROP;
					drop.push(next_event);
					break;
				}

				if(generator.empty()) {
					next_event.type = GENERATOR_DEQ;
					generator_service_time = expon(GENERATOR_RATE);
					next_event.next_event_time = simulation_time + generator_service_time;
					events.push(next_event);
				}	
				generator.push(next_event);
			
				
				// to_print = events;
				// print_the_queue(to_print);
				// to_print = generator;
				// print_the_queue(to_print);
					
				break;

			case GENERATOR_DEQ:
				cout << line_number++ << "§ I am a dequeue generator." << endl;
				cout << "\t" << next_event << endl;
				cout << "\t" << "Global Clock Time " << simulation_time << endl;
				cout << "-------------------------------------------------------" << endl<< endl;
				
				next_event.generator_deq = simulation_time;
				next_event.type = WAITING_ENQ;
				events.push(next_event);
				generator.pop();
					
				
				if(!generator.empty())
				{
					Request next_request = generator.front();
					// generator.pop();
					next_request.type = GENERATOR_DEQ;
					generator_service_time = expon(GENERATOR_RATE);
					next_request.next_event_time = simulation_time + generator_service_time;
					events.push(next_request);
					// waiting.push(next_event);
				}

				generator_completed++;
				// to_print = events;
				// print_the_queue(to_print);
				// to_print = generator;
				// print_the_queue(to_print);
				
				break;

			case WAITING_ENQ:
				cout << line_number++ << "§ I am a enqueue waiting." << endl;
				cout << "\t" << next_event << endl;
				cout << "\t" << "Global Clock Time " << simulation_time << endl;
				cout << "-------------------------------------------------------" << endl<< endl;
				
				next_event.waiting_enq = simulation_time; 
				if(waiting.empty()) {
					next_event.type = WAITING_DEQ;
					next_event.next_event_time = simulation_time;
					events.push(next_event);
				}
				waiting.push(next_event);
				break;

			case WAITING_DEQ:
				cout << line_number++ << "§ I am a dequeue waiting." << endl;
				cout << "\t" << next_event << endl;
				cout << "\t" << "Global Clock Time " << simulation_time << endl;
				cout << "-------------------------------------------------------" << endl<< endl;
				
				next_event.waiting_deq = simulation_time;
				next_event.type = CHECKER_ENQ;
				events.push(next_event);
				waiting.pop();
				

				if(!waiting.empty())
				{
					Request next_request = waiting.front();
					next_request.type = WAITING_DEQ;
					next_request.next_event_time = simulation_time + 1/SERVER_RATE + 1/CHECKER_RATE;
					events.push(next_request);
					// waiting.push(next_event);
				}
				
				break;

			case CHECKER_ENQ:
				cout << line_number++ << "§ I am a enqueue checker." << endl;
				cout << "\t" << next_event << endl;
				cout << "\t" << "Global Clock Time " << simulation_time << endl;
				cout << "-------------------------------------------------------" << endl<< endl;
				next_event.checker_enq = simulation_time;

				if(checker.size() >= checker_queue_size) {	
					// next_event.type = DROP;
					drop.push(next_event);
					break;
				}

				if(checker.empty()) {
					next_event.type = CHECKER_DEQ;
					checker_service_time = expon(CHECKER_RATE);
					next_event.next_event_time = simulation_time + checker_service_time;
					events.push(next_event);
				
				}
				checker.push(next_event);
				
				break;

			case CHECKER_DEQ:
				cout << line_number++ << "§ I am a dequeue checker." << endl;
				cout << "\t" << next_event << endl;
				cout << "\t" << "Global Clock Time " << simulation_time << endl;
				cout << "-------------------------------------------------------" << endl<< endl;
				
				next_event.checker_deq = simulation_time;
				next_event.type = SERVER_ENQ;
				events.push(next_event);
				checker.pop();
				

				if(!checker.empty())
				{
					Request next_request = checker.front();
					next_request.type = CHECKER_DEQ;
					checker_service_time = expon(CHECKER_RATE);
					next_request.next_event_time = simulation_time + checker_service_time;
					events.push(next_request);
					// waiting.push(next_event);
				}
				
				checker_completed++;
				break;

			case SERVER_ENQ:
				cout << line_number++ << "§ I am a enqueue server." << endl;
				cout << "\t" << next_event << endl;
				cout << "\t" << "Global Clock Time " << simulation_time << endl;
				cout << "-------------------------------------------------------" << endl<< endl;
				next_event.server_enq = simulation_time;

				if(server.size() >= server_queue_size) {	
					// next_event.type = DROP;
					drop.push(next_event);
					break;
				}

				if(server.empty()) {
					next_event.type = SERVER_DEQ;
					next_event.next_event_time = simulation_time + expon(SERVER_RATE);
					events.push(next_event);
				}
				server.push(next_event);

				break;

			case SERVER_DEQ:
				cout << line_number++ << "§ I am a dequeue server." << endl;
				cout << "\t" << next_event << endl;
				cout << "\t" << "Global Clock Time " << simulation_time << endl;
				cout << "-------------------------------------------------------" << endl<< endl;
				
				next_event.server_deq = simulation_time;
				next_event.type = DONE;
				events.push(next_event);
				server.pop();
				

				if(!server.empty())
				{
					Request next_request = server.front();
					next_request.type = SERVER_DEQ;
					server_service_time = expon(SERVER_RATE);
					next_request.next_event_time = simulation_time + server_service_time;
					events.push(next_request);
					// waiting.push(next_event);
				}
				server_completed++;
				break;
			case DONE:
				cout << "DONE" << endl;
				done.push(next_event);
				break;
			default:
				cout << "§ Error processing event";
				return INT_MIN;

		}
	}

	//copy to new queue
	to_print = done;
	cout << "Done Queue:" << endl;
	while(!to_print.empty())
	{	
			Request request_done = to_print.front();
			cout << request_done << request_done.show_times() << endl;
			to_print.pop();
	}


	to_print = drop;
	cout << "Drop Queue:" << endl;
	while(!to_print.empty())
	{	
			Request request_drop = to_print.front();
			cout << request_drop << request_drop.show_times() << endl;
			to_print.pop();
	}


	to_print = done;
	cout << " Webq Response Time: " << endl;
	while(!to_print.empty())
	{	
			Request request_done = to_print.front();
			cout << request_done.id << " " << request_done.server_deq - request_done.checker_enq << endl;
			to_print.pop();
	}


	to_print = done;
	cout << " Generators Response Time: " << endl;
	
	while(!to_print.empty())
	{	
		Request request_done = to_print.front();
			//deq request
		cout << request_done.id << "  " 
	       	<< request_done.generator_deq - request_done.generator_enq << endl;
		to_print.pop();
	}

	to_print = drop;
	// cout << " Dropped Generators Response Time: " << endl;
	
	while(!to_print.empty())
	{	
		Request request_drop = to_print.front();
		if(request_drop.type == 4)
		{
			//deq request
		cout << request_drop.id << "  " 
				<< request_drop.generator_deq - request_drop.generator_enq << endl;
		}
		to_print.pop();
	}

	to_print = done;
	cout << " Checkers Response Time: " << endl;
	
	while(!to_print.empty())
	{	
		Request request_done = to_print.front();
			//deq request
		cout << request_done.id << "  " 
	       	<< request_done.checker_deq - request_done.checker_enq << endl;
		to_print.pop();
	}

	to_print = drop;
	// cout << " Dropped Generators Response Time: " << endl;
	
	while(!to_print.empty())
	{	
		Request request_drop = to_print.front();
		if(request_drop.type == 8)
		{
			//deq request
		cout << request_drop.id << "  " 
				<< request_drop.checker_deq - request_drop.checker_enq << endl;
		}
		to_print.pop();
	}
	
	to_print = done;
	cout << " Server's Response Time: " << endl;
	
	while(!to_print.empty())
	{	
		Request request_done = to_print.front();
			//deq request
		cout << request_done.id << "  " 
	       	<< request_done.server_deq - request_done.server_enq << endl;
		to_print.pop();
	}

	// cout << " Generators Throughput: " << generator_completed / << endl;
	
	return 0;
}
