// AClock.cpp

#include "plugin.hpp"		

struct AClock : Module {

};

struct AClockWidget : ModuleWidget {

};

void AClock::process(const ProcessArgs& args) {

}

Model* modelAClock = createModel<AClock, AClockWidget>("AClock");