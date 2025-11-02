#pragma once
#include <chrono>
#include <iostream>


//TODO: verify if this works efficently or should i use the ptfs c timing functions

#define START_TIMER(func)\
 auto start##func = std::chrono::high_resolution_clock::now();

#define STOP_TIMER(func)\
  auto end##func = std::chrono::high_resolution_clock::now();\
  auto duration##func = std::chrono::duration_cast<std::chrono::microseconds>(end##func-start##func).count();\
  std::cout<<"The Time taken by "<<#func<<" is : "<< duration##func*1e-4<<" ms"<<std::endl;