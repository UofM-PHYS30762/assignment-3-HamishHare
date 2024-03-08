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
#include<algorithm> // for transform() function

using std::string;

/* ----
TODO:
 - ///Validate only electron/muons allowed? Be stricter with mass checking?
 - ///Introduce const-ness to variables and arguments where needed///
 - Setter function validation
 - ///Move velocity<c check to separate function so can be reused
 - Update velocity to be speed instead, as asked for my assignment
   (OR, ADD A COMMENT TO MY CODE SAYING WHAT I HAVE DONE INSTEAD)
 - Add a setter for beta directly which infers velocity.
 - ///Format outputs///
 - MOVE IMPLEMENTATION OF FUNCTIONS OUTSIDE THE CLASS
 - All other validation
 - ///Deal with regular/anti-particles better. Rename charge variables
      to reflect the multiples of e and add 'anti' onto the name when
      getting the type. Maybe restructure to instead have variable called
      'type' or 'family' and then a different getter that gets the name
      (formatted for anti- or not) rather than this type (used for checking).///
 - ///Keep track of counts of electrons and muons detected individually,
      rather than just total detections of both///
---- */

// Function to convert strings to lowercase
void to_lowercase(string& input_string)
{
  std::transform(input_string.begin(), input_string.end(), input_string.begin(),
                   [](unsigned char c){return std::tolower(c);});
}

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

  // Validation functions
  // .. particle type
  void validate_type()
  {
    if(!(particle_type=="electron" || particle_type=="muon"))
    {
      std::cout<<"Provided type "<<particle_type<<" is not 'electron' or 'muon'";
      throw std::invalid_argument("Invalid particle type.");
    }
  }
  // .. rest mass
  void validate_mass()
  {
    if(rest_mass<=0)
    {
      std::cout<<"Particle masses must be positive. You entered "<<rest_mass;
      throw std::invalid_argument("Invalid particle mass.");
    }
  }
  // .. charge
  void validate_charge()
  {
    if(std::abs(charge)!=1)
    {
      std::cout<<"Particle charges must be 1 or -1. You entered "<<charge;
      throw std::invalid_argument("Invalid particle charge.");
    }
  }
  // .. velocity
  void validate_velocity()
  {
    if(std::abs(velocity)>speed_of_light)
    {
      std::cout<<"Particle velocity "<<velocity<<" m/s exceeds the speed of light";
      throw std::invalid_argument("Invalid particle velocity.");
    }
  }

public:
  // Constructors
  // .. Default constructor
  particle() = default;
  // .. Parameterised constructor
  particle(const string& type, const double& mass, const int& charge_quanta,
           const double& particle_velocity) :
           particle_type{type}, rest_mass{mass}, charge{charge_quanta},
           velocity{particle_velocity}, beta{particle_velocity/speed_of_light}
  {
    // Make particle_type lowercase
    to_lowercase(particle_type);

    // Validation
    // .. check that the particle type is either "electron" or "muon"
    validate_type();
    // .. check that the particle speed doesn't exceed the speed of light
    validate_velocity();
    // .. check that the mass is not negative
    validate_mass();
    // .. check that the charge is either 1 or -1
    validate_charge();
  }

  // Destructor
  ~particle(){std::cout<<"Destroyed an "<<particle_type<<std::endl;} // DEBUG comment

  // Getter functions
  string get_type() const {return particle_type;}
  string get_name() const // adapt the particle name if it's an anti-particle
  {
    if(charge<0) return "anti"+particle_type; else return particle_type;
  }
  double get_rest_mass() const {return rest_mass;}
  int get_charge() const {return charge;}
  double get_velocity() const {return velocity;}
  double get_beta() const {return beta;}

  // Setter functions, to change value of data members
  // Make sure you check input validity before changing something
  // Hint: you can use the input checking functions you used in assignment 1
  void set_type(const string& type){particle_type = type; validate_type();}
  void set_rest_mass(const double& mass){rest_mass = mass; validate_mass();}
  void set_charge(const int& charge_quanta){charge = charge_quanta; validate_charge();}
  void set_velocity(const double& particle_velocity)
  {
    velocity = particle_velocity;
    validate_velocity();
    beta = velocity/speed_of_light;
  }

  // Function to print info about a particle
  void print_data() const;
};

// Implementation of print_data function for particle
void particle::print_data() const
{
  std::cout
  <<" -- Type: "<<particle_type<<std::endl
  <<" -- Rest Mass: "<<rest_mass<<" MeV"<<std::endl
  <<" -- Charge: "<<-charge<<" e"<<std::endl
  <<" -- Velocity: "<<velocity<<" m/s"<<std::endl
  <<" -- Beta: "<<beta<<std::endl;
}
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
  size_t electron_detection_count{0};
  size_t muon_detection_count{0};
  size_t total_particle_count{0};

  // Function to get on/off message depending on status
  string status_message() const
  {
    if(status) return "on"; else return "off";
  }

  // Function to check if the given particle can be detected
  // by this detector
  bool can_detect(const particle& particle) const
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

  // Validation functions
  // .. detector type
  void validate_type()
  {
    if(!(detector_type=="tracker" || detector_type=="calorimeter"
         || detector_type=="muon chamber"))
    {
      std::cout<<"Provided type "<<detector_type<<" is not 'tracker',"
      "'calorimeter' or 'muon chamber'";
      throw std::invalid_argument("Invalid detector type.");
    }
  }
  // .. counts
  void validate_count(const size_t& count)
  {
    if(count<0)
    {
      std::cout<<"Provided count "<<count<<" is less than zero";
      throw std::invalid_argument("Invalid count provided.");
    }
  }

public:
  // Constructors
  // .. Default constructor
  detector() = default;
  // .. Parameterised constructor
  detector(string type, bool initial_status) : detector_type{type}, status{initial_status}
  {
    // Validation
    // .. check that the detector type is valid
    validate_type();
  }

  // Destructor
  ~detector(){std::cout<<"Destroyed a "<<detector_type<<std::endl;} // DEBUG comment

  // Getters
  string get_detector_type() const {return detector_type;}
  bool get_status() const {return status;}
  size_t get_detection_count() const {return detection_count;}
  size_t get_electron_detection_count() const {return electron_detection_count;}
  size_t get_muon_detection_count() const {return muon_detection_count;}
  size_t get_total_particle_count() const {return total_particle_count;}

  // Setters
  // TODO: VALIDATION
  void set_detector_type(const string& type){detector_type = type; validate_type();}
  void turn_on(){status=true;}
  void turn_off(){status=false;}
  void set_detection_count(const size_t& new_count)
  {
    validate_count(new_count);
    detection_count = new_count;
    if(detection_count>total_particle_count)
    {
      std::cout<<"WARNING: Detection count set to a value large than"
      " the total count. The total count has been set to this value."
      <<std::endl;
      total_particle_count = new_count;
    }
  }
  void set_electron_detection_count(const size_t& new_count)
  {
    validate_count(new_count);
    electron_detection_count = new_count;
    detection_count = new_count + muon_detection_count;
    if(detection_count>total_particle_count)
    {
      std::cout<<"WARNING: Changing the electron detection count led to"
      " a combined detection count larger than the total count. The total"
      " count has been set to the new combined detection count"<<std::endl;
      total_particle_count = detection_count;
    }
  }
  void set_muon_detection_count(const size_t& new_count)
  {
    validate_count(new_count);
    muon_detection_count = new_count;
    detection_count = new_count + electron_detection_count;
    if(detection_count>total_particle_count)
    {
      std::cout<<"WARNING: Changing the muon detection count led to a"
      " combined detection count larger than the total count. The total"
      " count has been set to the new combined detection count"<<std::endl;
      total_particle_count = detection_count;
    }
  }
  void set_total_particle_count(const size_t& new_count)
  {
    validate_count(new_count);
    total_particle_count = new_count;
  }

  // Print the detector information
  void print_data() const;

  bool detect_particle(const particle& particle)
  {
    // Increase total particle count
    total_particle_count++;
    // Check if the detector is on and is able to detect the particle
    if(status && can_detect(particle))
    {
      // .. announce the detected particle
      std::cout<<particle.get_name()<<" was detected"<<std::endl;
      // .. update counts
      detection_count++;
      if(particle.get_type()=="electron") electron_detection_count++;
      else if (particle.get_type()=="muon") muon_detection_count++;
      return true; // detected
    }
    return false; // undetected
  }
};

// Separate print_data() implementation for detector
void detector::print_data() const
{
  std::cout
  <<" -- Status: "<<status_message()<<std::endl
  <<" -- Detected particles: "<<detection_count<<std::endl
  <<"   (Electrons: "<<electron_detection_count
  <<", Muons: "<<muon_detection_count<<")"<<std::endl
  <<" -- Total particle count: "<<total_particle_count<<std::endl;
}

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
