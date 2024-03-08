// PHYS 30762 Programming in C++
// Assignment 3
// Practice with C++ classes and their interactions
// Hamish Hare

#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<cstdlib> // for abs() function
#include <stdexcept> // for throwing exceptions

using std::string;

// Beginning of particle class
class particle
{
private:
  const double speed_of_light = 2.99792458e8; // in m/s, speed of light in vacuum

  string particle_name{"electron"};
  double rest_mass{0.51099895}; // in Mev
  int charge{1};
  double velocity{0};
  double beta{velocity/speed_of_light};

public:
  // Constructors
  // .. Default constructor
  particle() = default;
  // .. Parameterised constructor
  particle(string name, double mass, int charge_quanta, double particle_velocity) :
    particle_name{name}, rest_mass{mass}, charge{charge_quanta}, velocity{particle_velocity},
    beta{particle_velocity/speed_of_light}
  {
    // Validation
    // .. check that the particle speed doesn't exceed the speed of light
    if(std::abs(particle_velocity)>speed_of_light)
      throw std::invalid_argument(
      "Velocity " + std::to_string(particle_velocity) + " m/s exceeds the speed of light");
    // .. check that the mass is not negative
    if(mass<0) throw std::invalid_argument(
      "Particle masses cannot be negative. You entered " + std::to_string(mass));
  }

  // Destructor
  ~particle(){std::cout<<"Destroyed an "<<particle_name<<std::endl;} // DEBUG comment

  // Getter functions
  string get_name(){return particle_name;}
  double get_rest_mass(){return rest_mass;}
  double get_charge(){return charge;}
  double get_velocity(){return velocity;}
  double get_beta(){return beta;}

  // Setter functions, to change value of data members
  // Make sure you check input validity before changing something
  // Hint: you can use the input checking functions you used in assignment 1

  // Function to print info about a particle
  void print_data();

};

// Implementation of print_data function goes here

// End of particle class and associated member functions

// Beginning of detector class

// Functionalities needed, in addition to constructor/destructor/setters/getters (see slides on BB):
// - write a function that takes a particle 
//   and returns a bool and a print-out on whether this type of detector detected the particle
//   depending on the detector/particle combination (see slides on BB)
// - have data members that keep track of how many particles were detected by this particular detector, 
//   and of which type
// - write a function (accessor) that prints how many particles passed through this detector

// End of detector class

// Main program
int main()
{
  // Constants
  double electron_rest_mass{0.51099895}; // MeV
  double muon_rest_mass{105.6583755}; // MeV

  particle e1;
  particle e2("electron", 0.511, 1, 1213131.0);
  //particle e3("electron", 0.511, 1, -3.12012e9); // Exceed speed of light
  //particle e4("electron", -0.511, 1, 3.12012e7); // Negative mass

  std::cout<<"Name: "<<e1.get_name()<<" Mass: "<<e1.get_rest_mass()<<
  " Charge: "<<e1.get_charge()<<" Vel: "<<e1.get_velocity()<<
  " Beta: "<<e1.get_beta()<<std::endl;

  // Create the following particles: 
  // two electrons, four muons, one antielectron, one antimuon
  // Use the parameterised constructor

  // Print out the data from all the particles (put them in a vector)

  // Create the following detectors: a tracker, a calorimeter, a muon chamber

  // Pass the list of particles into each detector

  // Print a summary of how many particles were detected

  return 0;
}
