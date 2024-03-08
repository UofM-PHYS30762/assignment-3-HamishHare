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
 - All other validation
 - Deal with regular/anti-particles better. Rename charge variables
   to reflect the multiples of e and add 'anti' onto the name when
   getting the type. Maybe restructure to instead have variable called
   'type' or 'family' and then a different getter that gets the name
   (formatted for anti- or not) rather than this type (used for checking).
---- */

// Beginning of particle class
class particle
{
private:
  const double speed_of_light = 2.99792458e8; // in m/s, speed of light in vacuum

  string particle_type{"electron"};
  double rest_mass{0.51099895}; // in Mev
  int charge{1}; // multiples of -e
  double velocity{0};
  double beta{velocity/speed_of_light};

public:
  // Constructors
  // .. Default constructor
  particle() = default;
  // .. Parameterised constructor
  particle(string type, double mass, int charge_quanta, double particle_velocity) :
    particle_type{type}, rest_mass{mass}, charge{charge_quanta}, velocity{particle_velocity},
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
  ~particle(){std::cout<<"Destroyed an "<<particle_type<<std::endl;} // DEBUG comment

  // Getter functions
  string get_type(){return particle_type;}
  string get_name() // adapt the particle name if it's an anti-particle
  {
    if(charge<0) return "anti"+particle_type; else return particle_type;
  }
  double get_rest_mass(){return rest_mass;}
  int get_charge(){return charge;}
  double get_velocity(){return velocity;}
  double get_beta(){return beta;}

  // Setter functions, to change value of data members
  // Make sure you check input validity before changing something
  // Hint: you can use the input checking functions you used in assignment 1
  // TODO: VALIDATION
  void set_type(const string& type){particle_type = type;}
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
    std::cout
    <<" -- Type: "<<particle_type<<std::endl
    <<" -- Rest Mass: "<<rest_mass<<" MeV"<<std::endl
    <<" -- Charge: "<<-charge<<" e"<<std::endl
    <<" -- Velocity: "<<velocity<<" m/s"<<std::endl
    <<" -- Beta: "<<beta<<std::endl;
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
    string particle_type = particle.get_type();
    // .. tracker can detect both electrons and muons
    if(detector_type=="tracker" &&
      (particle_type=="electron" || particle_type=="muon")) return true;
    // .. calorimeter can detect electrons
    else if(detector_type=="calorimeter" && particle_type=="electron") return true;
    // .. muon chamber can detect muons
    else if(detector_type=="muon chamber" && particle_type=="muon") return true;
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
    std::cout
    <<" -- Status: "<<status_message()<<std::endl
    <<" -- Detected particles: "<<detection_count<<std::endl
    <<" -- Total particle count: "<<total_particle_count<<std::endl;
  }

  int detect_particle(particle& particle)
  {
    // Increase total particle count
    total_particle_count++;
    // Check if the detector is on and is able to detect the particle
    if(status && can_detect(particle))
    {
      detection_count++;
      std::cout<<particle.get_name()<<" was detected"<<std::endl;
      return 1;
    }
    return 0;
  }
};
// End of detector class

// Function to run a vector of particles through a detector
void pass_particles_through_detector(detector& current_detector,
                                     std::vector<particle>& particles)
{
  // .. turn detector on
  current_detector.turn_on();
  // .. pass particles through it
  for(auto particle{particles.begin()}; particle<particles.end(); ++particle)
  {
    current_detector.detect_particle(*particle);
  }
  // .. turn detector off
  current_detector.turn_off();
}

// Main program
int main()
{
  // Constants
  const double electron_rest_mass{0.51099895}; // MeV
  const double muon_rest_mass{105.6583755}; // MeV

  // Create a vector of particles:
  // two electrons, four muons, one antielectron, one antimuon
  std::vector<particle> particles;

  particles.emplace_back("electron", electron_rest_mass, 1, 41.21);
  particles.emplace_back("electron", electron_rest_mass, 1, -2.01e8);
  particles.emplace_back("muon", muon_rest_mass, 1, 34324.12);
  particles.emplace_back("muon", muon_rest_mass, 1, -612.5421);
  particles.emplace_back("muon", muon_rest_mass, 1, 9.6233e3);
  particles.emplace_back("muon", muon_rest_mass, 1, 0.56);
  particles.emplace_back("electron", electron_rest_mass, -1, 1.74e7);
  particles.emplace_back("muon", muon_rest_mass, -1, 0.0);

  // Print out the data from all the particles
  std::cout<<"============================="<<std::endl
           <<"          PARTICLES"<<std::endl
           <<"============================="<<std::endl;
  int i{1};
  for(auto particle{particles.begin()}; particle<particles.end(); ++particle)
  {
    std::cout<<"Particle "<<i<<" ("<<(*particle).get_name()<<"):"<<std::endl
             <<"-----------------------------"<<std::endl;
    (*particle).print_data();
    std::cout<<"-----------------------------"<<std::endl;
    i++;
  }

  // Create a vector of detectors:
  // a tracker, a calorimeter, a muon chamber
  std::vector<detector> detectors;

  detectors.emplace_back("tracker", false);
  detectors.emplace_back("calorimeter", false);
  detectors.emplace_back("muon chamber", false);

  // Pass the list of particles into each detector
  std::cout<<std::endl
           <<"============================="<<std::endl
           <<"          DETECTION"<<std::endl
           <<"============================="<<std::endl;
  for(auto detector{detectors.begin()}; detector<detectors.end(); ++detector)
  {
    // .. print which detector is currently in use
    std::cout<<"For the "<<(*detector).get_detector_type()<<":"<<std::endl;
    std::cout<<"-----------------------------"<<std::endl;
    // .. pass the particles through it
    pass_particles_through_detector((*detector), particles);
    std::cout<<"-----------------------------"<<std::endl;
  }

  // Print a summary of how many particles were detected
  std::cout<<std::endl
           <<"============================="<<std::endl
           <<"           SUMMARY"<<std::endl
           <<"============================="<<std::endl;
  // .. loop over each detector
  for(auto detector{detectors.begin()}; detector<detectors.end(); ++detector)
  {
    std::cout<<"For the "<<(*detector).get_detector_type()<<":"<<std::endl
             <<"-----------------------------"<<std::endl;
    (*detector).print_data();
    std::cout<<"-----------------------------"<<std::endl;
  }

  return 0;
}
