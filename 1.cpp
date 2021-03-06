#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <algorithm>
#include <ctime>

using namespace std;

mutex sum_lock;

const int thread_count = 10;
const int array_size = 1e8;
const int modul = 1e9 + 7;

long long array_mul = 1;

vector<int> my_array(array_size, 0);

void thread_function(int left, int right)
{
	long long local_mul = 1;
	for (int i = left; i < right; ++i) 
	{
		if ((local_mul * my_array[i]) % modul != 0) {
			local_mul = (my_array[i] * local_mul) % modul;
		}
		local_mul *= my_array[i];
	}
	lock_guard<mutex> _output_lock(sum_lock);
	cout << "Thread #" << this_thread::get_id() << " is done..." << endl;
	if ((array_mul * local_mul) % modul != 0) {
		array_mul = (local_mul * array_mul) % modul;
	}
	array_mul *= local_mul;
}

int main()
{
	srand(time(nullptr));
	generate(my_array.begin(), my_array.end(), rand);
	auto start_time = clock();

	vector<thread> ths(thread_count);
	int cur_counter = 0, cur_left = 0, cur_right = 0;
	int block_size = array_size / thread_count;

	for (auto &th : ths)
	{
		cur_left = cur_counter*block_size;
		cur_right = cur_left + block_size;
		th = thread(thread_function, cur_left, cur_right);
		cur_counter++;
	}
	if (array_size % thread_count != 0) {
		thread th = thread(thread_function, cur_counter*block_size, array_size);
		th.join();
	}

	for (auto &th : ths)
		th.join();
	
	auto end_time = clock();
	cout << "Time: " << (end_time - start_time) / 1000.0 << endl;

	return 0;
}
