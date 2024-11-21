#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <raylib.h>

#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

#include "common/constants.h"

class SortVisualizer {
   private:
    std::mt19937 generator;

   public:
    // Array that is going to be sorted
    int arr[300] = {0};

    int bogo_arr_size = 5;

    // Array that is going to be used to visualize bogo sort
    std::vector<int> bogo_arr;

    // Always sorted array.
    int reference[300] = {0};

    // Self explanatory
    bool isSorting = false;

    // Current step of the sorting algorithm
    int current_step = 0;

    // Function that is going to be called to sort the array
    void (SortVisualizer::*sorting_function)();

    // Maximum digit of the array (used for radix sort)
    int radix_maxDigit = 10;

    // Current digit of the array (used for radix sort)
    int radix_currentDigit = 0;

    // Buckets used for radix sort
    std::vector<std::vector<int>> radix_buckets;

    // Stack used for quick sort pivots
    std::vector<int> iter_quick_stack;

    // Size of the array that is going to be merged (used for merge sort)
    int merge_arr_size = 1;

    // Start of the left array that is going to be merged (used for merge sort)
    int merge_left_start = 1;

    // Steps per second
    int steps_per_second = 144;

    std::chrono::steady_clock::time_point last_step_time;

    static const int RADIX = 10;

    SortVisualizer() : generator(std::random_device{}()) {
        this->bogo_arr_size = 5;
        this->init_array();
        this->shuffle_array();
        this->sorting_function = &SortVisualizer::bubble_sort_step;
        this->steps_per_second = 144;
        this->last_step_time = std::chrono::steady_clock::now();
        this->radix_buckets.resize(RADIX);
    };

    void Draw();
    void Update();
    void bubble_sort_step();
    void selection_sort_step();
    void insertion_sort_step();
    void quick_sort_step();
    void merge_sort_step();
    void heap_sort_step();
    void radix_lsd_sort_step();
    void bogo_sort_step();

   private:
    void init_array();
    void shuffle_array();
    void handle_input();
};

#endif
