#include "simulation.h"

void Simulation::run(double endTime, double sampleTime) {
	for( double time = 0; time < endTime; time += sampleTime ) {
		runStep(time);
	}
}
	
	
void Simulation::runStep(double time) {
	printf("%f", time);
	for(Processors::iterator i = _processors.begin(); 
		i != _processors.end(); i++ ) {
		IProcessor* p = *i;
		p->update(time);
		if( _outputs.find(p) != _outputs.end() )
		printf(", %f", p->get() );
	}
	printf("\n");		
}
