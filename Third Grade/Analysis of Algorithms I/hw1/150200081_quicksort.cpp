/* @Author
Student Name: Kaan Karataş
Student ID : 150200081
Date: 23.11.2023
*/

#include <iostream>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <chrono>

// Struct to represent city data
struct CityData 
{
    std::string city_name;
    int population;
};

// Method for swaping the values of two CityData structs
void swap_city_values(CityData& first, CityData& second) {
    // Swap city names
    std::string temp_city_name = first.city_name;
    first.city_name = second.city_name;
    second.city_name = temp_city_name;

    // Swap populations
    int temp_city_population = first.population;
    first.population = second.population;
    second.population = temp_city_population;
}

// Method for reading city data from a CSV file
std::vector<CityData> read_csv(const std::string &dataset_file_name)
{
    std::vector<CityData> city_dataset;

    std::ifstream csv_file(dataset_file_name);
    if (!csv_file.is_open())
    {
        // Display an error message if the file cannot be opened
        std::cerr << "File error" << std::endl;
        return city_dataset;
    }

    std::string line;
    while (std::getline(csv_file, line))
    {
        // Parse the CSV line and create CityData structs
        std::istringstream iss(line);
        std::string city_name;
        std::string city_pop_str;

        if (std::getline(iss, city_name, ';') && std::getline(iss, city_pop_str, ';'))
        {
            CityData city_data;
            city_data.city_name = city_name;
            city_data.population = std::stoi(city_pop_str);
            city_dataset.push_back(city_data);
        }
    }

    // Close the CSV file
    csv_file.close();
    return city_dataset;
}

// Method for performing insertion sort on a vector of CityData structs
void insertionsort(std::vector<CityData>& dataset, const int& left, const int& right) {
    for(int j = left + 1; j <= right; ++j) {
        CityData key = dataset[j];
        int i = j - 1;

        // Move elements greater than key to one position ahead of their current position
        while (i >= left && dataset[i].population > key.population) {
            swap_city_values(dataset[i + 1], dataset[i]);
            i -= 1;
        }
        dataset[i + 1] = key;
    }
}

// Method for choosing the pivot index based on the chosen pivoting strategy
int choose_pivot(std::vector<CityData>& dataset, const int& left, const int& right, char pivoting_strategy) {
    if(pivoting_strategy == 'l')    return right;
    else if(pivoting_strategy == 'r')   return left + rand() % (right - left + 1);
    else if(pivoting_strategy == 'm') {
        // Choose the pivot as the average of three random elements
        int random_elements[3] = {left + rand() % (right - left + 1), left + rand() % (right - left + 1), left + rand() % (right - left + 1)};
        return (random_elements[0] + random_elements[1] + random_elements[2]) / 3;
    }
    
    return right;
}

// Method for saving the sub-arrays after each partition in a .txt file
void print_log(const std::vector<CityData> dataset, const int& left, const int& right, std::ofstream& log) {
        log << "Pivot: " << dataset[0].population << " List: [";
        for(int i = left; i <= right; ++i)   log << dataset[i].population << ", ";
        log << dataset[dataset.size() - 1].population << "]" << std::endl;
}

// Method for parting the dataset for the quicksort algorithm
int partition(std::vector<CityData>& dataset, const int& left, const int& right, char& pivoting_strategy, bool& verbose, std::ofstream& log) {
    int pivot_index = choose_pivot(dataset, left, right, pivoting_strategy);

    // Move the pivot to the end
    swap_city_values(dataset[pivot_index], dataset[right]);
    int j = left - 1;

    // Iterate through the elements and move elements smaller than the pivot to the left
    for(int i = left; i < right; ++i) {
        if(dataset[i].population <= dataset[right].population) {
            j += 1;
            swap_city_values(dataset[j], dataset[i]);
        }
    }

    // Move the pivot to its final position
    swap_city_values(dataset[j + 1], dataset[right]);

    if(verbose) print_log(dataset, left, right, log);
    
    return j + 1;
}

// Method for performing the quicksort algorithm on the dataset
void quicksort(std::vector<CityData>& dataset, int left, int right, char pivoting_startegy, const int& threshold, bool& verbose, std::ofstream& log) {
    if(left >= right)   return;

    int array_size = right - left + 1;

    // If the array size is smaller than or equal to the threshold, use insertion sort instead
    if(array_size <= threshold) {
        insertionsort(dataset, left, right);
    }
    else {
        int mid = partition(dataset, left, right, pivoting_startegy, verbose, log);

        // Recursively call quicksort on left and right subarrays
        quicksort(dataset, left, mid - 1, pivoting_startegy, threshold, verbose, log);
        quicksort(dataset, mid + 1, right, pivoting_startegy, threshold, verbose, log);
    }
}

void save_and_print_dataset(const std::vector<CityData> &dataset, const std::string &output_name, const char& pivoting_strategy, const int& threshold, const auto& sorting_duration)
{
    // Open a file for writing the sorted dataset
    std::ofstream output_file(output_name);
    
    // Write information of cities to the file using a for-each loop
    for (const auto& city: dataset) {
        output_file << city.city_name << ";" << city.population << "\n";
    }

    // Display sorting details, including the chosen pivoting strategy, threshold, and sorting duration
    std::cout << "Time taken by Quicksort with pivot strategy '" << pivoting_strategy << "' and threshold " << threshold << ": " << sorting_duration << " in ns." << std::endl;

    // Close the output file
    output_file.close();
}

int main(int argc, char *argv[])
{
    // Check if the correct number of command-line arguments is provided
    if (argc < 5 || argc > 6) {
        std::cerr << "Usage: " << argv[0] << " <dataset_file_name> <pivoting_strategy> <threshold> <output_name> [v]\n";
        return 1;
    }

    // Extract command-line arguments
    std::string dataset_file_name = argv[1];
    char pivoting_strategy = argv[2][0];
    int threshold = std::stoi(argv[3]);
    std::string output_name = argv[4];
    bool verbose = (argc == 6 && argv[5][0] == 'v');

    std::ofstream log("log.txt", verbose ? std::ofstream::out : std::ofstream::trunc);

    std::vector<CityData> city_dataset = read_csv(dataset_file_name);

    // Measure the time before sorting
    auto start_time = std::chrono::high_resolution_clock::now();    

    quicksort(city_dataset, 0, city_dataset.size() - 1, pivoting_strategy, threshold, verbose, log);

    // Measure the time after sorting
    auto end_time = std::chrono::high_resolution_clock::now();
    auto sorting_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

    save_and_print_dataset(city_dataset, output_name, pivoting_strategy, threshold, sorting_duration);

    log.close();

    return 0;
}