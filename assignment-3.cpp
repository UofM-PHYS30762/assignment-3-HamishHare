// PHYS 30762 Programming in C++
// Assignment 3
// Practice with C++ classes and their interactions
// Hamish Hare

#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<sstream>
#include<cstdlib> // for abs() function
#include<stdexcept> // for throwing exceptions

using std::string;

/* ----
TODO:
 - Validate only electron/muons allowed? Be stricter with mass checking?
 - Introduce const-ness to variables and arguments where needed
 - Setter function validation
 - Move velocity<c check to separate function so can be reused
 - Format outputs
 - MOVE IMPLEMENTATION OF FUNCTIONS OUTSIDE THE CLASS
---- */

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
  int get_charge(){return charge;}
  double get_velocity(){return velocity;}
  double get_beta(){return beta;}

  // Setter functions, to change value of data members
  // Make sure you check input validity before changing something
  // Hint: you can use the input checking functions you used in assignment 1
  // TODO: VALIDATION
  void set_name(const string& name){particle_name = name;}
  void set_rest_mass(const double& mass){rest_mass = mass;}
  void set_charge(const int& charge_quanta){charge = charge_quanta;}
  void set_velocity(const double& particle_velocity)
  {
    velocity = particle_velocity; // VALIDATION (<c) !!!!
    beta = velocity/speed_of_light;
  }
  //void set_beta(const double& name){return beta;}

  // Function to print info about a particle
  void const print_data()
  {
    std::cout<<"--------------------------------"<<std::endl;
    std::cout<<"Particle:"<<std::endl
    <<" -- Type: "<<particle_name<<std::endl
    <<" -- Mass: "<<rest_mass<<" MeV"<<std::endl
    <<" -- Charge: "<<charge<<std::endl
    <<" -- Velocity: "<<velocity<<" m/s"<<std::endl
    <<" -- Beta: "<<beta<<std::endl;
    std::cout<<"--------------------------------"<<std::endl;

    // std::ostringstream output_stream;

    // output_stream<<"Particle details:"<<std::endl
    // <<" -- Type: "<<particle_name<<std::endl
    // <<" -- Rest mass: "<<rest_mass<<" MeV"<<std::endl
    // <<" -- Charge: "<<charge<<std::endl
    // <<" -- Velocity: "<<velocity<<" m/s"<<std::endl
    // <<" -- Beta: "<<beta<<std::endl;
    //course_name = course_name_stream.str();
    //course_name_stream.str(""); // clear stream
  }

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

class detector
{
private:
  string detector_type{"tracker"};
  bool status{false}; // true <--> on, false <--> off
  size_t detection_count{0};
  size_t total_particle_count{0};

  // Function to get on/off message depending on status
  string const status_message()
  {
    if(status) return "on"; else return "off";
  }

  // Function to check if the given particle can be detected
  // by this detector
  bool const can_detect(particle& particle)
  {
    string particle_name = particle.get_name();
    // .. tracker can detect both electrons and muons
    if(detector_type=="tracker" &&
      (particle_name=="electron" || particle_name=="muon")) return true;
    // .. calorimeter can detect electrons
    else if(detector_type=="calorimeter" && particle_name=="electron") return true;
    // .. muon chamber can detect muons
    else if(detector_type=="muon chamber" && particle_name=="muon") return true;
    // .. otherwise undetected
    return false;
  }

public:
  // Constructors
  // .. Default constructor
  detector() = default;
  // .. Parameterised constructor
  detector(string type, bool initial_status) : detector_type{type}, status{initial_status}
  {
    // Validation
    // TODO!!
    // .. check that the detector type is valid
  }

  // Destructor
  ~detector(){std::cout<<"Destroyed a "<<detector_type<<std::endl;} // DEBUG comment

  // Getters
  string get_detector_type(){return detector_type;}
  bool get_status(){return status;}
  size_t get_detection_count(){return detection_count;}
  size_t get_total_particle_count(){return total_particle_count;}

  // Setters
  // TODO: VALIDATION
  void set_detector_type(const string& type){detector_type = type;}
  void turn_on(){status=true;}
  void turn_off(){status=false;}
  void set_detection_count(const size_t& new_count)
  {
    detection_count = new_count;
    if(detection_count>total_particle_count)
    {
      std::cout<<"WARNING: Detection count set to a value large than"
      " the total count, updating the total count to match"<<std::endl;
      total_particle_count = new_count;
    }
  }
  void set_total_particle_count(const size_t& new_count)
       {total_particle_count = new_count;}

  // Print the detector information
  void const print_data()
  {
    std::cout<<"--------------------------------"<<std::endl;
    std::cout<<"Detector:"<<std::endl
    <<" -- Type: "<<detector_type<<std::endl
    <<" -- Status: "<<status_message()<<std::endl
    <<" -- Detected: "<<detection_count<<std::endl
    <<" -- Total Count: "<<total_particle_count<<std::endl;
    std::cout<<"--------------------------------"<<std::endl;
  }

  int detect_particle(particle& particle)
  {
    // Increase total particle count
    total_particle_count++;
    // Check if the detector is on and is able to detect the particle
    if(status && can_detect(particle))
    {
      detection_count++;
      std::cout<<particle.get_name()<<" was detected by a "<<detector_type<<std::endl;
      return 1;
    }
    return 0;
  }
};

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
  //e2.print_data();

  detector d1;
  detector d2("muon chamber", true);
  d2.set_detection_count(20);

  particle m1("muon", muon_rest_mass, -1, 6.943e7);

  d2.print_data();
  d2.detect_particle(m1);
  d2.print_data();
  d2.turn_off();
  d2.detect_particle(m1);
  d2.print_data();
  d2.detect_particle(e2);
  d2.print_data();

  // Create the following particles: 
  // two electrons, four muons, one antielectron, one antimuon
  // Use the parameterised constructor
  // Create paricles:


  // Print out the data from all the particles (put them in a vector)

  // Create the following detectors: a tracker, a calorimeter, a muon chamber

  // Pass the list of particles into each detector

  // Print a summary of how many particles were detected

  return 0;
}
