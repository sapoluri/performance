#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

constexpr int size = static_cast<int>(1e8);
template<typename _InputIterator, typename _OutputIterator, typename _Tp,
           typename _BinaryOperation>
    _OutputIterator
    exclusive_scan_local(_InputIterator __first, _InputIterator __last,
                   _OutputIterator __result, _Tp __init,
                   _BinaryOperation __binary_op)
    {
      while (__first != __last)
        {
          auto __v = __init;
          __init = __binary_op(__init, *__first);
          ++__first;
          *__result++ = std::move(__v);
        }
      return __result;
    }

void  compute_exclusive_scan(vector<int>& input, vector<int>& output) {
    output[0] = 0;

    for (int i = 1; i < size; i++) {
        output[i] = input[i-1] + output[i - 1];
    }
}

void  compute_exclusive_scan(int *input, int *output) {
    output[0] = 0;

    for (int i = 1; i < size; i++) {
        output[i] = input[i-1] + output[i - 1];
    }
}

void  compute_exclusive_scan_static(int (&input)[size], int (&output)[size]) {
    output[0] = 0;

    for (int i = 1; i < size; i++) {
        output[i] = input[i-1] + output[i - 1];
    }
}

void run_vector() {
	vector<int> data(size);
	for (int i=0;i<size;i++) {
		data[i]=rand();
	}
	vector<int> output(size);
	auto begin = std::chrono::high_resolution_clock::now();
	exclusive_scan_local(data.begin(), data.end(), output.begin(), 0, plus<>{});
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

	cout << "Run time = "<< elapsed.count() * 1e-6 << " ms\n";

	begin = std::chrono::high_resolution_clock::now();
	compute_exclusive_scan(data, output);
	end = std::chrono::high_resolution_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
	cout << "Run time = "<< elapsed.count() * 1e-6 << " ms\n";
}

void run_dynamic_array() {
	int *input = new int[size], *out = new int[size];
	for (int i=0;i<size;i++) {
		input[i]=rand();
	}

	auto begin = std::chrono::high_resolution_clock::now();
	compute_exclusive_scan(input, out);
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
	cout << "Run time = "<< elapsed.count() * 1e-6 << " ms\n";
	delete[] input;
	delete[] out;
}

void run_static_array() {
	int input[size], out[size];
	for (int i=0;i<size;i++) {
		input[i]=rand();
	}

	auto begin = std::chrono::high_resolution_clock::now();
	compute_exclusive_scan_static(input, out);
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
	cout << "Run time = "<< elapsed.count() * 1e-6 << " ms\n";
}

int main() {
	run_vector();
	run_dynamic_array();
	run_static_array();
}
