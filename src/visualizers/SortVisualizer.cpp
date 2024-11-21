#include <SortVisualizer.hpp>
#include <algorithm>
#include <cmath>
#include <vector>

#include "common/constants.h"
#include "utils/sorting_utils.h"

void SortVisualizer::Draw() {
    int ActualWindowWidth = WINDOW_WIDTH;
    int ActualWindowHeight = WINDOW_HEIGHT;
    if (IsWindowFullscreen()) {
        ActualWindowWidth = GetScreenWidth();
        ActualWindowHeight = GetScreenHeight();
    }
    if (this->sorting_function == &SortVisualizer::bogo_sort_step) {
        // Bogosort path
        for (int i = 0; i < bogo_arr_size; i++) {
            int on_screen_size = ROW_MAX_HEIGHT / bogo_arr_size * bogo_arr[i];
            Color color;
            int reference_idx = this->reference[bogo_arr[i]];
            float hue = float(reference_idx) / float(bogo_arr_size);
            color = ColorFromHSV(hue * 360, 1, 1);
            DrawRectangle(
                50 + static_cast<int>(i * (ActualWindowWidth / bogo_arr_size)) +
                    (ActualWindowWidth - WINDOW_WIDTH) / 2,
                ActualWindowHeight - static_cast<int>(on_screen_size),
                static_cast<int>(ActualWindowWidth / bogo_arr_size),
                static_cast<int>(on_screen_size), color);
        }
        return;
    }
    for (int i = 0; i < ROW_COUNT; i++) {
        int on_screen_size = ROW_MAX_HEIGHT / ROW_COUNT * arr[i];
        Color color;
        int reference_idx = this->reference[arr[i]];
        float hue = float(reference_idx) / float(ROW_COUNT);
        color = ColorFromHSV(hue * 360, 1, 1);
        DrawRectangle(50 + static_cast<int>(i * 5) +
                          (ActualWindowWidth - WINDOW_WIDTH) / 2,
                      ActualWindowHeight - static_cast<int>(on_screen_size), 5,
                      static_cast<int>(on_screen_size), color);
    }
}

void SortVisualizer::Update() {
    this->handle_input();
    if (this->isSorting) {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                            now - last_step_time)
                            .count();
        if (duration >= (1000 / steps_per_second)) {
            (this->*sorting_function)();
            last_step_time = now;
        }
    }
}

void SortVisualizer::bubble_sort_step() {
    if (this->current_step <= ROW_COUNT - 1) {
        bool swapped = false;
        for (int i = 0; i < ROW_COUNT - this->current_step - 1; i++) {
            if (this->arr[i] > this->arr[i + 1]) {
                std::swap(this->arr[i], this->arr[i + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            this->isSorting = false;
        }
        this->current_step++;
    } else {
        this->isSorting = false;
    }
}

void SortVisualizer::selection_sort_step() {
    if (this->current_step < ROW_COUNT - 1) {
        int min_idx = this->current_step;
        for (int i = this->current_step + 1; i < ROW_COUNT; i++) {
            if (this->arr[i] < this->arr[min_idx]) {
                min_idx = i;
            }
        }
        std::swap(this->arr[min_idx], this->arr[this->current_step]);
        this->current_step++;
    } else {
        this->isSorting = false;
    }
}

void SortVisualizer::insertion_sort_step() {
    if (this->current_step < ROW_COUNT) {
        int key = this->arr[this->current_step];
        int j = this->current_step - 1;
        while (j >= 0 && this->arr[j] > key) {
            this->arr[j + 1] = this->arr[j];
            j--;
        }
        this->arr[j + 1] = key;
        this->current_step++;
    } else {
        this->isSorting = false;
    }
}

void SortVisualizer::quick_sort_step() {
    if (this->iter_quick_stack.size() == 0) {
        iter_quick_stack.push_back(0);
        iter_quick_stack.push_back(ROW_COUNT - 1);
    }

    if (this->iter_quick_stack.size() > 0) {
        int high = iter_quick_stack[iter_quick_stack.size() - 1];
        iter_quick_stack.pop_back();
        int low = iter_quick_stack[iter_quick_stack.size() - 1];
        iter_quick_stack.pop_back();
        int pivot = quick_partition(this->arr, low, high);

        if (pivot - 1 > low) {
            iter_quick_stack.push_back(low);
            iter_quick_stack.push_back(pivot - 1);
        }

        if (pivot + 1 < high) {
            iter_quick_stack.push_back(pivot + 1);
            iter_quick_stack.push_back(high);
        }
    }
    this->current_step++;
}

void SortVisualizer::merge_sort_step() {
    int n = ROW_COUNT;

    // Initialize merge_arr_size and merge_left_start if this is the first step
    if (current_step == 0) {
        this->merge_arr_size = 1;
        this->merge_left_start = 0;
    }

    // Perform one merge operation
    if (this->merge_arr_size < n) {
        if (this->merge_left_start < n - 1) {
            int mid = std::min(
                this->merge_left_start + this->merge_arr_size - 1, n - 1);
            int rightEnd = std::min(
                this->merge_left_start + 2 * this->merge_arr_size - 1, n - 1);
            merge_arrs(arr, this->merge_left_start, mid, rightEnd);
            this->merge_left_start += 2 * this->merge_arr_size;
        } else {
            // Move to the next size of subarrays
            this->merge_arr_size *= 2;
            this->merge_left_start = 0;
        }
    } else {
        this->isSorting = false;  // Sorting is complete
    }

    this->current_step++;
}

void SortVisualizer::heap_sort_step() {
    int n = ROW_COUNT;

    if (this->current_step == 0) {
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(this->arr, n, i);
        }
    }

    if (n > 1 && this->current_step < n) {
        std::swap(this->arr[0], this->arr[n - this->current_step - 1]);
        heapify(this->arr, n - 1 - this->current_step, 0);
    }

    this->current_step++;
}

void SortVisualizer::radix_lsd_sort_step() {
    if (current_step == 0) {
        // Initialize buckets
        radix_buckets.resize(RADIX);

        // Find the maximum number to know the number of digits
        int max_num = *std::max_element(arr, arr + ROW_COUNT);
        radix_maxDigit = 0;
        while (max_num > 0) {
            max_num /= 10;
            radix_maxDigit++;
        }

        radix_currentDigit = 0;
    }

    if (radix_currentDigit < radix_maxDigit) {
        // Clear buckets
        for (auto& bucket : radix_buckets) {
            bucket.clear();
        }

        // Place numbers in buckets
        for (int i = 0; i < ROW_COUNT; i++) {
            int digit =
                (arr[i] / static_cast<int>(std::pow(10, radix_currentDigit))) %
                10;
            radix_buckets[digit].push_back(arr[i]);
        }

        // Collect numbers from buckets
        int index = 0;
        for (const auto& bucket : radix_buckets) {
            for (int num : bucket) {
                arr[index++] = num;
            }
        }

        radix_currentDigit++;
    } else {
        isSorting = false;  // Sorting is complete
    }

    current_step++;
}

void SortVisualizer::bogo_sort_step() {
    if (!std::is_sorted(this->bogo_arr.begin(), this->bogo_arr.end())) {
        std::shuffle(this->bogo_arr.begin(), this->bogo_arr.end(), generator);
    } else {
        this->isSorting = false;
    }
}

void SortVisualizer::init_array() {
    for (int i = 0; i < ROW_COUNT; i++) {
        arr[i] = i + 1;  // Use i + 1 to avoid 0
        reference[i] = i + 1;
    }

    bogo_arr.clear();
    for (int i = 0; i < bogo_arr_size; i++) {
        bogo_arr.push_back(i + 1);
    }
}

void SortVisualizer::shuffle_array() {
    for (int i = 0; i < 300; i++) {
        int random_index = rand() % 300;
        std::swap(arr[i], arr[random_index]);
    }
    for (int i = 0; i < bogo_arr_size; i++) {
        int random_index = rand() % bogo_arr_size;
        std::swap(bogo_arr[i], bogo_arr[random_index]);
    }
}

void SortVisualizer::handle_input() {
    if (IsKeyPressed(KEY_SPACE)) {
        this->isSorting = !this->isSorting;
    }
    if (IsKeyPressed(KEY_R)) {
        this->init_array();
        this->shuffle_array();
        this->current_step = 0;
        this->isSorting = false;
        this->steps_per_second = -1;
    }
    if (IsKeyPressed(KEY_S)) {
        this->shuffle_array();
        this->current_step = 0;
        this->isSorting = false;
    }
    if (IsKeyPressed(KEY_B)) {
        this->sorting_function = &SortVisualizer::bubble_sort_step;
        this->shuffle_array();
        this->current_step = 0;
        this->isSorting = false;
    }
    if (IsKeyPressed(KEY_L)) {
        this->sorting_function = &SortVisualizer::selection_sort_step;
        this->shuffle_array();
        this->current_step = 0;
        this->isSorting = false;
    }
    if (IsKeyPressed(KEY_D)) {
        this->sorting_function = &SortVisualizer::radix_lsd_sort_step;
        this->shuffle_array();
        this->radix_currentDigit = 0;
        this->isSorting = false;
    }
    if (IsKeyPressed(KEY_I)) {
        this->sorting_function = &SortVisualizer::insertion_sort_step;
        this->shuffle_array();
        this->current_step = 0;
        this->isSorting = false;
    }
    if (IsKeyPressed(KEY_H)) {
        this->sorting_function = &SortVisualizer::heap_sort_step;
        this->shuffle_array();
        this->current_step = 0;
        this->isSorting = false;
    }
    if (IsKeyPressed(KEY_Q)) {
        this->sorting_function = &SortVisualizer::quick_sort_step;
        this->shuffle_array();
        this->current_step = 0;
        this->isSorting = false;
    }
    if (IsKeyPressed(KEY_M)) {
        this->sorting_function = &SortVisualizer::merge_sort_step;
        this->shuffle_array();
        this->current_step = 0;
        this->isSorting = false;
    }
    if (IsKeyPressed(KEY_G)) {
        this->sorting_function = &SortVisualizer::bogo_sort_step;
        this->shuffle_array();
        this->current_step = 0;
        this->isSorting = false;
    }
    if (IsKeyPressed(KEY_F)) {
        this->steps_per_second = -1;
    }
    if (IsKeyPressed(KEY_UP)) {
        this->steps_per_second /= 1.1;  // Increase speed
        if (this->steps_per_second < 0) {
            this->steps_per_second *= -1;
        }
    }
    if (IsKeyPressed(KEY_DOWN)) {
        this->steps_per_second *= 1.1;  // Decrease speed
        if (this->steps_per_second < 0) {
            this->steps_per_second *= -1;
        }
    }
    if (IsKeyPressed(KEY_F11)) {
        int monitor = GetCurrentMonitor();
        if (IsWindowFullscreen()) {
            SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
        } else {
            SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        }
        ToggleFullscreen();
    }
    if (IsKeyPressed(KEY_ONE)) {
        if (this->bogo_arr_size > 1) {
            this->bogo_arr_size--;
            this->bogo_arr.resize(this->bogo_arr_size);
            this->init_array();
            this->shuffle_array();
        }
    }
    if (IsKeyPressed(KEY_TWO)) {
        this->bogo_arr_size++;
        this->bogo_arr.resize(this->bogo_arr_size);
        this->init_array();
        this->shuffle_array();
    }
}
