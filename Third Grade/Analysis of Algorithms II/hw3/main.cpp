/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 3
 * Kaan Karataş
 * 150200081
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <map>

/* YOU CAN INCLUDE ANY OTHER NECESSARY LIBRARIES */

/* YOU CAN DEFINE EXTRA FUNCTIONS AND MODIFY THE CODE TEMPLATE */

/* YOU HAVE TO CONSIDER THE OUTPUT FORMAT TO TEST YOUR CODE WITH CALICO */

using namespace std;

struct TimeIntervals
{
    string floor_name;
    string room_no;
    string start_time;
    string end_time;
};

struct Priority
{
    string floor_name;
    string room_no;
    int room_priority;
};

struct Item
{
    string name;
    int price;
    float value;
};

/* START: WEIGHTED INTERVAL SCHEDULING (for each floor) */

struct Schedule
{
    TimeIntervals interval;
    Priority priority;

    // Overload < operator for sorting
    bool operator<(const Schedule &other) const
    {
        return interval.end_time < other.interval.end_time;
    }
};

// function to check if two schedules are conflicting
bool is_conflicting(const Schedule &s1, const Schedule &s2)
{
    return stoi(s1.interval.end_time) > stoi(s2.interval.start_time);
}

// function to find the latest non-conflicting schedule
int latest_non_Conflicting(const vector<Schedule> &schedules, int i)
{
    for (int j = i - 1; j >= 0; j--)
    {
        if (!is_conflicting(schedules[j], schedules[i]))
        {
            return j;
        }
    }
    return -1;
}

// function to find the best schedule for each floor
vector<Schedule> weighted_interval_scheduling(vector<Schedule> &schedules)
{
    vector<Schedule> optimal_solution = {}; // Vector for optimal solution

    if(schedules.size() == 0) return optimal_solution;  // Return empty if no schedules

    sort(schedules.begin(), schedules.end());   // Sort schedules by end time

    vector<int> max_priority(schedules.size(), 0);  // Vector for max priority for schedules
    max_priority[0] = schedules[0].priority.room_priority;  // Set first schedule's priority as max priority

    for (size_t i = 1; i < schedules.size(); i++)
    {
        int latest_non_conflicting_index = latest_non_Conflicting(schedules, i);    // Find latest non-conflicting schedule
        int priority = schedules[i].priority.room_priority; // Set current schedule's priority as the priority
        if (latest_non_conflicting_index != -1)
        {
            priority += max_priority[latest_non_conflicting_index]; // Add max priority of latest non-conflicting schedule
        }
        max_priority[i] = max(priority, max_priority[i - 1]);   // Set max priority for current schedule
    }

    int reverse_index = max_priority.size() - 1;    // Start from last schedule
    while (reverse_index >= 0)
    {
        if (reverse_index == 0 || max_priority[reverse_index] != max_priority[reverse_index - 1])
        {
            optimal_solution.push_back(schedules[reverse_index]);   // Add schedule to optimal solution
            reverse_index = latest_non_Conflicting(schedules, reverse_index);   // Find latest non-conflicting schedule
            continue;
        }
        reverse_index--;    // Move to previous schedule
    }

    reverse(optimal_solution.begin(), optimal_solution.end());  // Reverse optimal solution

    return optimal_solution;    // Return optimal solution
}

// *** END ***

/* START: KNAPSACK (for items) */

// function to select the most valuable items that can be purchased with a certain budget
vector<Item> knapsack(const vector<Item> &items, int budget)
{
    vector<vector<float>> dp(items.size() + 1, vector<float>(budget + 1, 0.0)); // 2D vector for dynamic programming

    for (size_t i = 1; i <= items.size(); ++i)  // Iterate over items
    {
        for (int j = 1; j <= budget; ++j)   // Iterate over budget
        {
            if (items[i - 1].price <= j)
            {
                dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - items[i - 1].price] + items[i - 1].value);   // Calculate max value
            }
            else
            {
                dp[i][j] = dp[i - 1][j];    // Set previous value
            }
        }
    }

    vector<Item> selectedItems; // Vector for selected items
    int i = items.size();   // Start from last item
    int j = budget; // Start from budget
    while (i > 0 && j > 0)  // Iterate over items and budget
    {
        if (dp[i][j] != dp[i - 1][j])   // If value is different
        {
            selectedItems.push_back(items[i - 1]);  // Add item to selected items
            j -= items[i - 1].price;    // Decrease budget
        }
        i--;    // Move to previous item
    }

    return selectedItems;   // Return selected items
}

// function to fill schedules
void fill_schedules(const map<string, vector<TimeIntervals>> &roomIntervals, const map<string, Priority> &priorities, map<string, vector<Schedule>> &schedules)
{
    for (const auto &interval: roomIntervals)   // Iterate over room intervals
    {
        for (const auto &timeInterval: interval.second) // Iterate over time intervals
        {
            Schedule schedule;  
            schedule.interval = timeInterval;   // Set interval
            schedule.priority = priorities.at(interval.first + timeInterval.room_no);   // Set priority

            schedules[interval.first].push_back(schedule);  // Add schedule to schedules
        }
    }
}

// function to read room time intervals from file
map<string, vector<TimeIntervals>> read_room_time_intervals(const string &filename)
{
    map<string, vector<TimeIntervals>> intervals;
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        getline(file, line); // Skip the first line
        while (getline(file, line))
        {
            stringstream ss(line);
            TimeIntervals interval;
            getline(ss, interval.floor_name, ' '); // Read FloorName
            getline(ss, interval.room_no, ' ');    // Read RoomNo
            getline(ss, interval.start_time, ' '); // Read Start
            getline(ss, interval.end_time, ' ');   // Read End
            intervals[interval.floor_name].push_back(interval); // Add interval to intervals
        }
        file.close();
    }
    return intervals;
}

// function to read priorities from file
map<string, Priority> read_priorities(const string &filename)
{
    map<string, Priority> priorities;
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        getline(file, line); // Skip the first line
        while (getline(file, line))
        {
            stringstream ss(line);
            Priority priority;
            getline(ss, priority.floor_name, ' '); // Read FloorName
            getline(ss, priority.room_no, ' ');    // Read RoomNo
            ss >> priority.room_priority;               // Read Priority
            priorities[priority.floor_name + priority.room_no] = priority;  // Add priority to priorities
        }
        file.close();
    }
    return priorities;
}

// function to read items from file
vector<Item> read_items(const string &filename)
{
    vector<Item> items;
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        getline(file, line); // Skip the first line

        while (getline(file, line))
        {
            stringstream ss(line);
            Item item;
            getline(ss, item.name, ' ');    // Read Name
            ss >> item.price;   // Read Price
            ss.ignore();
            ss >> item.value;   // Read Value
            items.push_back(item);  
        }
        file.close();
    }
    return items;
}

// function to print selected items
void print_selected_items(const vector<Item> &selectedItems)
{
    float totalValue = 0.0; // Total value initialized as 0
    for (const auto &item : selectedItems)  // Iterate over selected items
    {
        totalValue += item.value;   // Add value to total value
    }
    cout << "Total Value --> " << totalValue << endl;   // Print total value
    for (const auto &item : selectedItems)  // Iterate over selected items
    {
        cout << item.name << endl;  // Print item name
    }
}

// function to print schedule
void print_schedule(vector<Schedule> &schedules)
{
    int totalValue = 0; // Total value initialized as 0
    for (const auto &schedule : schedules)  // Iterate over schedules
    {
        totalValue += schedule.priority.room_priority;  // Add priority to total value
    }

    // Print floor name and priority gain
    cout << schedules[0].priority.floor_name << " --> Priority Gain: " << fixed << setprecision(1) << round(totalValue) << endl;

    for (const auto &schedule : schedules)  // Iterate over schedules
    {
        cout << schedule.interval.floor_name + "\t";    // Print floor name
        cout << schedule.interval.room_no + "\t";   // Print room no

        cout << setw(2) << setfill('0') << schedule.interval.start_time << "\t";    // Print start time
        cout << setw(2) << setfill('0') << schedule.interval.end_time << "\t" << endl;  // Print end time
    }
}

// *** END ***

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <case_number>" << endl;
        return 1;
    }

    string case_no = argv[1];
    string case_name = "case_" + case_no;
    string path = "./inputs/" + case_name;

    int total_budget = 200000;

    // Read input data
    map<string, vector<TimeIntervals>> roomIntervals = read_room_time_intervals(path + "/room_time_intervals.txt");
    map<string, Priority> priorities = read_priorities(path + "/priority.txt");
    vector<Item> items = read_items(path + "/items.txt");

    map<string, vector<Schedule>> schedules;    // Map for schedules
    fill_schedules(roomIntervals, priorities, schedules); // Fill schedules

    cout << "Best Schedule for Each Floor" << endl;
    for (auto &schedule : schedules)    // Iterate over schedules
    {
        schedule.second = weighted_interval_scheduling(schedule.second);    // Calculate best schedule
    }

    for(auto &schedule: schedules)  // Iterate over schedules
    {
        print_schedule(schedule.second);    // Print schedule
    }

    cout << "Best Use of Budget" << endl;
    vector<Item> selectedItems = knapsack(items, total_budget); // Calculate the best items
    print_selected_items(selectedItems);    // Print selected items

    return 0;
}