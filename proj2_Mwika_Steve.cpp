/*
Name: Steve Mwika
Project Number:Project 2
*/

#include <iostream>
#include <random>
#include <queue>
#include <map>
#include <ctime> // Include for time()

using namespace std;

struct Event {
    double time;
    int type; 
    int sender;
    int receiver;
    bool operator<(const Event& e) const {
        return time > e.time; // Min heap for priority queue
    }
};

class Model {
public:
    Model(double lossProbability) : lossProbability(lossProbability) {
        srand(time(nullptr));
    }

    void run_Model() {
        double c_Time = 0;
        int total_Fails = 0, total_Successes = 0;
        map<pair<int, int>, double> sent_Hello_Times; // Tracks the time a Hello was sent from sender to receiver

        while (c_Time < 2000.0) {
            double n_Time = c_Time + expont_Dist(5.0);
            for (int i = 0; i < 5; ++i) {
                int receiver = rand() % 5;
                while (receiver == i) receiver = rand() % 5; // Ensure sender != receiver
                eventQueue.push({n_Time, 0, i, receiver});
                sent_Hello_Times[{i, receiver}] = n_Time; // Track when the Hello was sent
                eventQueue.push({n_Time + 2.0, 2, i, receiver});
            }

            while (!eventQueue.empty() && eventQueue.top().time <= c_Time) {
                auto event = eventQueue.top();
                eventQueue.pop();
                if (event.type == 0) { // Hello
                    cout << "node." << event.sender << " sends a Hello to node." << event.receiver << " at " << event.time << " seconds." << endl;
                    if (rand() % 100 >= lossProbability * 100) {
                        eventQueue.push({event.time + 0.3, 1, event.receiver, event.sender}); // Add processing delay
                    }
                } else if (event.type == 1) { // Hello_Ack
                    cout << "node." << event.receiver << " receives a Hello_Ack from node." << event.sender << " at " << event.time << " seconds." << endl;
                    total_Successes++;
                    sent_Hello_Times.erase({event.receiver, event.sender});
                } else if (event.type == 2) { // Timeout
                    if (sent_Hello_Times.find({event.sender, event.receiver}) != sent_Hello_Times.end()) {
                        cout << "Transmission from node." << event.sender << " to node." << event.receiver << " failed after timeout." << endl;
                        total_Fails++;
                        sent_Hello_Times.erase({event.sender, event.receiver});
                    }
                }
            }

            c_Time += 1.0; 
        }

        cout << "Simulation ends. Total successes: " << total_Successes << ", Total failures: " << total_Fails << endl;
    }

private:
// A priority queue that stores 'Event' objects.
    priority_queue<Event> eventQueue;
    double lossProbability;

    double expont_Dist(double mean) {
        // pseudorandom number generator (mt19937).
        random_device rd;
        // A Mersenne Twister pseudorandom number generator. It is seeded with a value from 'rd' to ensure
        mt19937 gen(rd());
        exponential_distribution<> d(1.0 / mean);
        // This value represents the time until the next event in the simulation.
        return d(gen);
    }
};

int main() {

    double lossProbability = 0.1;

    Model sim(lossProbability);
    sim.run_Model();

    /*
    while(lossProbability <= 0.5)
    {
        //cout << "Running simulation with lossProbability = " << lossProbability << endl;
        for(int i = 0; i < 5; i++)
        {
            sleep(1);
            Simulator sim(lossProbability);
            sim.runSimulation();
        }
        lossProbability += 0.1;
    }
    */
    
    
}
