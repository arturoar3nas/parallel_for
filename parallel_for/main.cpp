#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <thread>
#include <vector>
#include <algorithm>

#include "tbb/parallel_for_each.h"
#include "tbb/tick_count.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

int rnd() {
    return (rand() % 100);
}



int threadingbuildingblocks()
{
    int addition = 5;
    tbb::tick_count before = tbb::tick_count::now();

    std::vector<int> numbers(100000000);
    std::generate(numbers.begin(), numbers.end(), rnd);

    tbb::tick_count after = tbb::tick_count::now();
    std::printf("time spent (generate random): t%g seconds\n", (after - before).seconds());

    //std::for_each version
    before = tbb::tick_count::now();
    std::for_each(numbers.begin(), numbers.end(), [&] (int &v) { v += addition;});
    after = tbb::tick_count::now();
    std::printf("time spent (std::for_each): t%g seconds\n", (after - before).seconds());


    //tbb::parallel_for_each version
    before = tbb::tick_count::now();
    tbb::parallel_for_each(numbers.begin(), numbers.end(), [&] (int &v) { v += addition;});
    after = tbb::tick_count::now();
    std::printf("time spent (tbb::parallel_for_each): t%g seconds\n", (after - before).seconds());

    //tbb::parallel_for iterator version
    before = tbb::tick_count::now();
    tbb::parallel_for(

                tbb::blocked_range<std::vector<int>::iterator>(numbers.begin(),
                                                               numbers.end()),
                [&] (tbb::blocked_range<std::vector<int>::iterator> number) {
        for (std::vector<int>::iterator it = number.begin(); it != number.end(); it++) {
            (*it) += addition;
        }
    });

    after = tbb::tick_count::now();
    std::printf("time spent (tbb::parallel_for iterator): t%g seconds\n", (after - before).seconds());
    //tbb::parallel_for index version
    before = tbb::tick_count::now();
    tbb::parallel_for(size_t(0), size_t(numbers.size()),
                      [&] (size_t index) { numbers[index] += addition; });
    after = tbb::tick_count::now();
    std::printf("time spent (tbb::parallel_for index): t%g seconds\n",(after - before).seconds());
    return 0;
}

void standar_for()
{
    int N = 20;
    auto code = [](int start, int end)->void
    {
        int a = 2;
        int z[20] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
        for(int i = start; i < end; i++){
            z[i] = a*z[i];
            std::cout << "id thread: " << std::this_thread::get_id() <<  "    z[i]: " << std::to_string(z[i]) << "\n" << std::flush;
        }
    };

    std::thread t[std::thread::hardware_concurrency()];

    for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
        t[i] = std::thread(code, 0/*start*/, N/std::thread::hardware_concurrency()/*end*/);
    }

    for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
        t[i].join();
    }
}

static void show_usage(std::string name)
{
    std::cerr << "Usage: \n"
              << "Options:\n"
              << "\t-h,--help\t\tShow this help message\n"
              << "\t-i,--intel"
              << "\t-std,--standar"
              << std::endl;
}


int main (int argc, char* argv[])
{
    if (argc < 1) {
        show_usage(argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
         std::string arg = argv[i];
         if ((arg == "-h") || (arg == "--help")) {
             show_usage(argv[0]);
             return 0;
         }
         if ((arg == "-i") || (arg == "--intel")) {
             threadingbuildingblocks();
         }
         if ((arg == "-std") || (arg == "--standar")) {
             standar_for();
         }
     }

    return 0;
}
