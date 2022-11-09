#include <iostream>
#include <fstream>

int main() {


  // Auslesen
  double wert_parallel;
  double wert_sequentiell;

  std::ifstream file("parallel.txt");
  while (file >> wert_parallel) {
    std::cout << "Parallel: Tp=" << wert_parallel << std::endl;
  }

  std::ifstream file1("sequentiell.txt");
  while (file1 >> wert_sequentiell) {
    std::cout << "Sequentiell: Ts=" << wert_sequentiell << std::endl;
  }

  double Speed_up;

  Speed_up=wert_sequentiell/wert_parallel;
  double cpu_cores=4;
  //grep "cpu cores" /proc/cpuinfo | uniq -> Terminal zum Abfragen der Kerne
  std::cout << "Parallelelitätsgewinn: S=" << Speed_up << std::endl;

  if (Speed_up == cpu_cores){
    std::cout << "Parallelitätsgewinn ist linear" << std::endl;
  } else if( Speed_up > cpu_cores){
    std::cout << "Parallelitätsgewinn ist superlinear" << std::endl;
  } else if( Speed_up < cpu_cores){
    std::cout << "Parallelitätsgewinn ist sublinear" << std::endl;
  }
  std::cout << "Info: E=1 -> r Parallele Algorithmus kostenoptimal" << std::endl;
  std::cout << "Effizienz: E=" << Speed_up/cpu_cores << std::endl;



  return 0;
}