/* @Author
Student Name: Kaan Karataş
Student ID : 150200081
Date: 14.12.2023
*/

#include <iostream>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <cmath>

// Struct to represent city data
struct CityData
{
    std::string city_name;
    int population;
};

// Method for swaping the values of two CityData structs
void swap_city_values(CityData &first, CityData &second)
{
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

// Method for maintaining the max heap property
void max_heapify(std::vector<CityData> &dataset, int size, int index)
{
    int largest = index;

    int left_child = 2 * index + 1;
    int right_child = 2 * index + 2;

    // Check if the left child is larger than the current largest
    if (left_child < size && dataset[left_child].population > dataset[largest].population)
    {
        largest = left_child;
    }

    // Check if the right child is larger than the current largest
    if (right_child < size && dataset[right_child].population > dataset[largest].population)
    {
        largest = right_child;
    }

    if (largest != index)
    {
        // Swap and recursively heapify
        swap_city_values(dataset[index], dataset[largest]);
        max_heapify(dataset, size, largest);
    }
}

// Method for building a max heap
void build_max_heap(std::vector<CityData> &dataset, int size)
{
    // Start from the last non-leaf node and heapify each node
    for (int i = (size - 1) / 2; i >= 0; i--)
        max_heapify(dataset, size, i);
}

// Method for performing heapsort
void heapsort(std::vector<CityData> &dataset, int size)
{
    // Build a max-heap from the input city dataset
    build_max_heap(dataset, size);

    // Extract elements from the max-heap
    for (int i = size - 1; i >= 1; i--)
    {
        swap_city_values(dataset[0], dataset[i]);
        max_heapify(dataset, i, 0);
    }
}

// Method for inserting a new element into the max heap
void max_heap_insert(std::vector<CityData> &dataset, int size, CityData &city_data)
{
    // Increase the size of the heap
    ++size;
    dataset.push_back(city_data);

    int index = size - 1;
    int parent = (index - 1) / 2;

    // Heapify to maintain the max heap property
    while (index > 0 && dataset[parent].population < dataset[index].population)
    {
        swap_city_values(dataset[index], dataset[parent]);
        index = parent;
        parent = (index - 1) / 2;
    }
}

// Method for extracting the maximum element from the max heap
CityData heap_extract_max(std::vector<CityData> &dataset, int size)
{
    // Swap the root (max) with the last element
    swap_city_values(dataset[0], dataset[size - 1]);
    --size;

    // Heapify to maintain the max heap property
    max_heapify(dataset, size, 0);

    // Return the extracted max element
    return dataset[size];
}

// Method for increasing the key of a heap element
void heap_increase_key(std::vector<CityData> &dataset, int size, int index, int key)
{
    if (key <= dataset[index].population)
    {
        std::cout << "New key is smaller than current key" << std::endl;
        return;
    }

    index--;
    dataset[index].population = key;

    int parent = (index - 1) / 2;

    // Heapify to maintain the max heap property
    while (index > 0 && dataset[parent].population < dataset[index].population)
    {
        swap_city_values(dataset[index], dataset[parent]);
        index = parent;
        parent = (index - 1) / 2;
    }
}

// Method for getting the maximum element from the max heap without extracting it
CityData heap_maximum(std::vector<CityData> &dataset)
{
    return dataset[0];
}

// Method for calculating the height of a d-ary heap
int dary_calculate_height(int size, int d)
{
    int height = 0;
    int temp = size;

    // Calculate the height based on the number of children (d)
    while (temp > 0)
    {
        temp -= pow(d, height);
        ++height;
    }
    return height -1;
}

// Method for maintaining the max heap property in a d-ary heap
void dary_max_heapify(std::vector<CityData> &dataset, int size, int index, int d)
{
    int largest = index;

    int first_child = d * index + 1;
    int last_child = d * index + d;

    // Find the largest child
    for (int i = first_child; i <= last_child && i < size; ++i)
    {
        if (dataset[i].population > dataset[largest].population)
        {
            largest = i;
        }
    }

    if (largest != index)
    {
        // Swap and recursively heapify
        swap_city_values(dataset[index], dataset[largest]);
        dary_max_heapify(dataset, size, largest, d);
    }
}

// Method for building a max heap for a d-ary heap
void dary_build_max_heap(std::vector<CityData> &dataset, int size, int d)
{
    // Start from the last non-leaf node and heapify each node
    for (int i = (size - 1) / 2; i >= 0; i--)
        dary_max_heapify(dataset, size, i, d);
}

// Method for extracting the maximum element from a d-ary heap
CityData dary_extract_max(std::vector<CityData> &dataset, int size, int d)
{
    // Swap the root with the last element
    swap_city_values(dataset[0], dataset[size - 1]);
    --size;

    // Heapify to maintain the max heap property
    dary_max_heapify(dataset, size, 0, d);

    // Return the extracted max element
    return dataset[size];
}

// Method for inserting a new element into a d-ary heap
void dary_insert_element(std::vector<CityData> &dataset, int size, CityData city_data, int d)
{
    // Build a max heap for the d-ary heap
    dary_build_max_heap(dataset, size, d);

    // Increase the size of the heap and add the new element
    ++size;
    dataset.push_back(city_data);

    int index = size - 1;
    int parent = (index - 1) / d;

    // Heapify to maintain the max heap property
    while (index > 0 && dataset[parent].population < dataset[index].population)
    {
        swap_city_values(dataset[index], dataset[parent]);
        index = parent;
        parent = (index - 1) / d;
    }
}

// Method for increasing the key of an element in a d-ary heap
void dary_increase_key(std::vector<CityData> &dataset, int size, int index, int key, int d)
{
    if (key <= dataset[index].population)
    {
        std::cout << "New key is smaller than current key" << std::endl;
        return;
    }

    index--;
    dataset[index].population = key;

    int parent = (index - 1) / d;

    // Heapify to maintain the max heap property
    while (index > 0 && dataset[parent].population < dataset[index].population)
    {
        swap_city_values(dataset[index], dataset[parent]);
        index = parent;
        parent = (index - 1) / d;
    }
}

// Method for extracting number from the input string
int extract_number(const std::string &str)
{
    std::string number_str;

    for (char ch : str)
    {
        if (std::isdigit(ch))
        {
            number_str += ch;
        }
        else if (!number_str.empty())
        {
            break;
        }
    }

    return !number_str.empty() ? std::stoi(number_str) : 0;
}

// Method for extracting city name from the input string
std::string extract_city_name(const std::string &str)
{
    std::stringstream ss(str);
    std::string token;
    std::string result;

    for (int i = 0; i < 2; i++)
    {
        std::getline(ss, token, '_');
        result = token;
    }

    return result;
}

// Method for extracting optional arguments
void extract_optional_args(int argc, char *argv[], int &d, int &i, int &k, std::string &city)
{
    // Parse optional arguments
    for (int arg_index = 4; arg_index < argc; ++arg_index)
    {
        std::string arg = argv[arg_index];

        // Check if the argument starts with 'd', 'i', or 'k'
        if (!arg.empty() && (arg[0] == 'd' || arg[0] == 'i' || arg[0] == 'k'))
        {
            // Extract the value following the letter
            int value = extract_number(arg);

            // Assign the value to the corresponding variable
            switch (arg[0])
            {
            case 'd':
                d = value;
                break;
            case 'i':
                i = value;
                break;
            case 'k':
                k = value;
                city = extract_city_name(arg);
                break;
            default:
                std::cerr << "Invalid optional argument: " << arg << std::endl;
            }
        }
        else
        {
            std::cerr << "Invalid optional argument: " << arg << std::endl;
        }
    }
}

// Method for performing the specified heap operation based on the function_name
void select_operation(const std::string &function_name, std::vector<CityData> &city_dataset, int i, int k, int d, std::string city)
{
    if (function_name == "max_heapify")
    {
        max_heapify(city_dataset, city_dataset.size(), i);
    }
    else if (function_name == "build_max_heap")
    {
        build_max_heap(city_dataset, city_dataset.size());
    }
    else if (function_name == "heapsort")
    {
        build_max_heap(city_dataset, city_dataset.size());
        heapsort(city_dataset, city_dataset.size());
    }
    else if (function_name == "max_heap_insert")
    {
        CityData new_city = {city, k};
        build_max_heap(city_dataset, city_dataset.size());
        max_heap_insert(city_dataset, city_dataset.size(), new_city);
    }
    else if (function_name == "heap_extract_max")
    {
        build_max_heap(city_dataset, city_dataset.size());
        CityData max = heap_extract_max(city_dataset, city_dataset.size());
        std::cout << "Max of heap: " << max.population << std::endl;
    }
    else if (function_name == "heap_increase_key")
    {
        build_max_heap(city_dataset, city_dataset.size());
        heap_increase_key(city_dataset, city_dataset.size(), i, k);
    }
    else if (function_name == "heap_maximum")
    {
        build_max_heap(city_dataset, city_dataset.size());
        heap_maximum(city_dataset);
        CityData max = heap_extract_max(city_dataset, city_dataset.size());
        std::cout << "Max of heap: " << max.population << std::endl;
    }
    else if (function_name == "dary_calculate_height")
    {
        dary_build_max_heap(city_dataset, city_dataset.size(), d);
        int height = dary_calculate_height(city_dataset.size(), d);

        std::cout << "Height of d-ary heap: " << height << std::endl;
    }
    else if (function_name == "dary_extract_max")
    {
        dary_build_max_heap(city_dataset, city_dataset.size(), d);
        dary_extract_max(city_dataset, city_dataset.size(), d);
        CityData max = heap_extract_max(city_dataset, city_dataset.size());
        std::cout << "Max of heap: " << max.population << std::endl;
    }
    else if (function_name == "dary_insert_element")
    {
        CityData new_city = {city, k};
        dary_build_max_heap(city_dataset, city_dataset.size(), d);
        dary_insert_element(city_dataset, city_dataset.size(), new_city, d);
    }
    else if (function_name == "dary_increase_key")
    {
        dary_build_max_heap(city_dataset, city_dataset.size(), d);
        dary_increase_key(city_dataset, city_dataset.size(), i, k, d);
    }
    else
    {
        std::cerr << "Invalid function name: " << function_name << std::endl;
    }
}

void save_dataset(const std::vector<CityData> &dataset, const std::string &output_name, const int d, const auto &sorting_duration)
{
    // Open a file for writing the sorted dataset
    std::ofstream output_file(output_name);

    // Write information of cities to the file using a for-each loop
    for (const auto &city : dataset)
    {
        output_file << city.city_name << ";" << city.population << "\n";
    }

    // Display sorting details
    std::cout << "Time taken by operation with " << d << " number of children for non-leaf nodes: " << sorting_duration << " in ns." << std::endl;

    // Close the output file
    output_file.close();
}

void save_single_dataset(const CityData &city, const std::string &output_name, const int d, const auto &sorting_duration)
{
    // Open a file for writing the sorted dataset
    std::ofstream output_file(output_name);

    // Write information of cities to the file using a for-each loop
    output_file << city.city_name << ";" << city.population << "\n";

    // Display sorting details
    std::cout << "Time taken by operation with " << d << " number of children for non-leaf nodes: " << sorting_duration << " in ns." << std::endl;

    // Close the output file
    output_file.close();
}

int main(int argc, char *argv[])
{
    // Check if the correct number of command-line arguments is provided
    if (argc < 4 || argc > 7)
    {
        std::cerr << "Usage: " << argv[0] << " <dataset_file_name> <function_name> <output_name> [dik]\n";
        return 1;
    }

    // Extract command-line arguments
    std::string dataset_file_name = argv[1];
    std::string function_name = argv[2];
    std::string output_name = argv[3];

    // Optional parameters
    int d = 2; // Default number of children for non-leaf nodes
    int i = 0; // Default index value
    int k = 0; // Default key value for increase_key functions

    std::string city = ""; // Default city name for insert function

    extract_optional_args(argc, argv, d, i, k, city);

    std::vector<CityData> city_dataset = read_csv(dataset_file_name);

    // Measure the time before sorting
    auto start_time = std::chrono::high_resolution_clock::now();

    select_operation(function_name, city_dataset, i, k, d, city);

    // Measure the time after sorting
    auto end_time = std::chrono::high_resolution_clock::now();
    auto sorting_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

    if (function_name == "heap_maximum" || function_name == "heap_extract_max" || function_name == "dary_extract_max")
    {
        save_single_dataset(city_dataset[0], output_name, d, sorting_duration);
    }
    else
        save_dataset(city_dataset, output_name, d, sorting_duration);

    return 0;
}