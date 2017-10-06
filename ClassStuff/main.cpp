#include <iostream>
#include <cmath>
#include <stdlib.h>
#include "solarsystem.h"
#include "euler.h"
#include "velocityverlet.h"
#include <fstream>
#include <iomanip>
using namespace std;

int main(int numArguments, char **arguments)
{
    double years = 5;
    int numTimesteps = years*10;
    double dt = years / (numTimesteps-1);
    if(numArguments >= 2) numTimesteps = atoi(arguments[1]);

    SolarSystem solarSystem;
    // We create new bodies like this. Note that the createCelestialBody function returns a reference to the newly created body
    // This can then be used to modify properties or print properties of the body if desired
    // Use with: solarSystem.createCelestialBody( position, velocity, mass );

    solarSystem.createCelestialBody( vec3(0,0,0), vec3(0,0,0), 1.0 ); // The sun
    solarSystem.createCelestialBody( vec3(1, 0, 0), vec3(0, 2*M_PI, 0), 3e-6 ); //The Earth

    // To get a list (a reference, not copy) of all the bodies in the solar system, we use the .bodies() function
    vector<CelestialBody> &bodies = solarSystem.bodies();

    for(int i = 0; i<bodies.size(); i++) {
        CelestialBody &body = bodies[i]; // Reference to this body
        cout << "The position of this object is " << body.position << " with velocity " << body.velocity << endl;
    }

    // advance all planets:

    //Euler integrator(dt);
    VelocityVerlet integrator(dt);

    solarSystem.writeToFile("test.txt");     // get initial values in text file as well
    solarSystem.calculateForcesAndEnergy();

    //write initial energy to file:
    solarSystem.calculateForcesAndEnergy();
    ofstream ofile;
    ofile.open("energy.txt",ofstream::app);
    ofile<<setw(15) << solarSystem.kineticEnergy() << solarSystem.potentialEnergy() <<endl;
    ofile<<endl;
    ofile.close();

    // forward loop
    for(int timestep=0; timestep<numTimesteps; timestep++) {
        integrator.integrateOneStep(solarSystem);
        solarSystem.writeToFile("test.txt");

        //write energy to file for each step to test conservation:
        solarSystem.calculateForcesAndEnergy();
        ofstream ofile;
        ofile.open("energy.txt",ofstream::app);
        ofile<<setprecision(30) << solarSystem.kineticEnergy() <<"  "<< setprecision(30) << solarSystem.potentialEnergy()<<"  " <<endl;
    }
    ofile<<endl;
    ofile.close();

    cout << "I just created my first solar system that has " << solarSystem.bodies().size() << " objects." << endl;
    return 0;
}

