#include <iostream>
#include <fstream>

int main() {
  // Variablen für Werte aus Dateien
  double wert_parallel, wert_sequentiell;
  
  // Dateien öffnen und Werte auslesen
  std::ifstream file("parallel.txt");
  if (file.is_open()) {
    file >> wert_parallel;
    file.close();
  } else {
    std::cerr << "Fehler: Konnte Datei 'parallel.txt' nicht öffnen!" << std::endl;
    return 1;
  }
  
  std::ifstream file1("sequentiell.txt");
  if (file1.is_open()) {
    file1 >> wert_sequentiell;
    file1.close();
  } else {
    std::cerr << "Fehler: Konnte Datei 'sequentiell.txt' nicht öffnen!" << std::endl;
    return 1;
  }
  
  // Speedup berechnen und ausgeben
  double Speed_up = wert_sequentiell / wert_parallel;
  std::cout << "Parallelelitätsgewinn: S=" << Speed_up << std::endl;
  
  // CPU-Kerne auslesen und Effizienz berechnen
  std::ifstream cpuinfo("/proc/cpuinfo");
  std::string line;
  int cpu_cores = 1;
  while (std::getline(cpuinfo, line)) {
    if (line.find("cpu cores") != std::string::npos) {
      cpu_cores = std::stoi(line.substr(line.find(":") + 2));
      break;
    }
  }
  double Effizienz = Speed_up / cpu_cores;
  std::cout << "Effizienz: E=" << Effizienz << std::endl;
  
  // Parallelitätsgewinn bestimmen und ausgeben
  if (Speed_up == cpu_cores) {
    std::cout << "Parallelitätsgewinn ist linear" << std::endl;
  } else if (Speed_up > cpu_cores) {
    std::cout << "Parallelitätsgewinn ist superlinear" << std::endl;
  } else {
    std::cout << "Parallelitätsgewinn ist sublinear" << std::endl;
  }
  
  std::cout << "Info: E=1 -> r Parallele Algorithmus kostenoptimal" << std::endl;

  return 0;
}
