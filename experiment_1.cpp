#include<iostream>
#include<thread>
#include<algorithm>
#include <cassert>
#include<functional>



void Incrementor( std::atomic<int> &sharedCounter, const int targetValue, int& executionCount, const int maxPerThread)
{
    // int thread1counter=0;
    // int thread2counter=0;
    int localIncrements=0;

    
    //int32_t numberOfThreads = 2;
  while(sharedCounter < targetValue && localIncrements < maxPerThread) {
    
    sharedCounter++;
    executionCount++;
    localIncrements++;

  }

}


int main ()
{
    int thread1ExecutionCounter;
    int thread2ExecutionCounter;
    int32_t targetValue = 1000000;
    int NUMBEROFTHREADS = 2;
    const int NUMBEROFITERATIONS= 100;
    int32_t maxPerThread = static_cast<int>(targetValue/NUMBEROFTHREADS);
   
    for (int32_t i = 0;i<NUMBEROFITERATIONS;++i) {
        thread1ExecutionCounter=0;
        thread2ExecutionCounter=0;
        std::atomic<int> sharedCounter{0};  
        std::thread thread2(Incrementor, std::ref(sharedCounter), std::ref(targetValue),std::ref(thread2ExecutionCounter),std::ref(maxPerThread));
      
        std::thread thread1(Incrementor, std::ref(sharedCounter), std::ref(targetValue), std::ref(thread1ExecutionCounter),std::ref(maxPerThread));

        thread1.join();
        thread2.join();
  
        if(sharedCounter.load()!=targetValue){
            std::cout<<"i= "<<i<<", Value of SharedCounter is: "<<sharedCounter<<std::endl;
            // add : numberofRuns vs NumberOftimes Shared counter overshot/UNDERSHOT then plot

        }
        
    }
        std::cout << "Thread 1 did " << thread1ExecutionCounter << " increments"<<std::endl;
        std::cout << "Thread 2 did " << thread2ExecutionCounter << " increments"<<std::endl;

    return 0;

}
