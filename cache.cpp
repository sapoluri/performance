//Some of the code below is derived from and written to verify information posted here:
//https://meribold.org/2017/10/20/survey-of-cpu-caches/#listing-1

#include <iostream>
#include <chrono>
#include <memory>

#define N 100000000  // 100 million

typedef struct elem {
   struct elem *next;
} elem;


void MemAccessLatency(int size, int shuffled=true) {
   auto array = std::make_unique<elem[]>(size);
   auto t1 = std::chrono::steady_clock::now();
   for (size_t i = 0; i < size - 1; ++i) {
      array[i].next = &array[i + 1];
   }

   array[size - 1].next = &array[0];
   // Fisher-Yates shuffle the array.
   if (shuffled) {
      //Need to shuffle to notice pure access latency, where cache locality and prefetching are useless.
      for (size_t i = 0; i < size - 1; ++i) {
         size_t j = i + rand() % (size - i);  // j is in [i, SIZE).
         elem temp = array[i];  // Swap array[i] and array[j].
         array[i] = array[j];
         array[j] = temp;
      }
   }

   auto t2 = std::chrono::steady_clock::now();
   //std::cout << "Setup time: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "us" <<  std::endl;

   int64_t dummy = 0;
   elem *i = &array[0];
   t1 = std::chrono::steady_clock::now();
   for (size_t n = 0; n < N; ++n) {
      dummy += (int64_t)i;
      i = i->next;
   }
   t2 = std::chrono::steady_clock::now();
   std::cout << "For size = " << size*sizeof(elem) << " bytes, Time taken per access " << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()/N << "ns" << std::endl;
}

int main() {
   int mem_size;
   std::cout << "Perform lscpu to find cache sizes\n\n";
   std::cout << "Enter L1 Cache size in Bytes: ";
   std::cin >> mem_size;
   std::cout << "\n";
   MemAccessLatency(mem_size/sizeof(elem), true); //Random memory access. Every single iteration causes a memory access.

   std::cout << "Enter L2 Cache Size in Bytes: ";
   std::cin >> mem_size;
   std::cout << "\n";
   MemAccessLatency(mem_size/sizeof(elem), true); //Random memory access. Every single iteration causes a memory access.

   std::cout << "Enter L3 Cache Size in Bytes: ";
   std::cin >> mem_size;
   std::cout << "\n";
   MemAccessLatency(mem_size/sizeof(elem), true); //Random memory access. Every single iteration causes a memory access.

   std::cout << "Enter a size greater than L3 Cache in Bytes: ";
   std::cin >> mem_size;
   std::cout << "\n";
   MemAccessLatency(mem_size/sizeof(elem), true); //Random memory access. Every single iteration causes a memory access.

   cout << "Measurements for linear array access:\n";
   MemAccessLatency(mem_size/sizeof(elem), false); //Linear memory access. Cache locality and prefecting result in accesses being simply L1 cache access. So access time per element is 1-2 ns.
}
