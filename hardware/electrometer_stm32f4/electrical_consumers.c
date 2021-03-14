#include "electrical_consumers.h"
#include "main.h"

void set_id_consumer(){
	if(event_control.ACTIVE_POWER > 590 && event_control.ACTIVE_POWER < 610){
		event_control.id_consumer = CONSUMER_LAMP;
	} else if(event_control.ACTIVE_POWER > 1590 && event_control.ACTIVE_POWER < 1610){
		event_control.id_consumer = CONSUMER_IRON;
	} else {
		event_control.id_consumer = 0;
	}
}