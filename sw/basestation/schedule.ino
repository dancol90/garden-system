/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

/*
    Update the jobs schedule, starting a new job if it's time, of ending it if needed.

    TODO: add support for rollover. Now a job cannot cross the midnight.
*/
void update_schedule() {
    int8_t min_index, next_index;
    long   min_value, next_value;
    Interval job;

    for (byte r = 0; r < RECEIVER_COUNT; r++) {
        // Before checking, set the current receiver as there are no jobs.
        // This will be overitten only if a job is actually found.
        // If not, it's ok for it to mantains this value.
        receivers[r].current_job = -1;

         min_index = -1;
        next_index = -1;
         min_value = 0xFFFF;
        next_value = 0xFFFF;

        for(byte i = 0; i < jobs_count; i++) {
            job = get_recv_job(r, i);

            // The lowest one but after the current time
            if (job.enabled) {
                if (job.start.minutes < min_value) {
                    // A smaller value than the last one was found. Save it
                    min_value = job.start.minutes;
                    min_index = i;
                }

                // If this job is later today, it's what we are searching for.
                if (job.start.minutes < next_value && job.start.minutes >= now.minutes) {
                    next_value = job.start.minutes;
                        next_index = i;
                    Serial.println(next_index);
                }
                
                bool condition = job.start.minutes <= now.minutes && now.minutes < job.end.minutes;

                if (receivers[r].active != condition) {
                    // Change the state only if is not forced by the user
                    if (!receivers[r].forced) {
                        receivers[r].active = condition;
                        write_tx_fifo();
                    }
                } else {
                    // Keep the forced state until it matches the scheduled one.
                    receivers[r].forced = false;
                }

                if (condition) {
                    receivers[r].current_job = i;

                    break;
                }
            }
        }

        // If no future jobs are found, return the absolute mininum
        // (that's the first of tomorrow) or -1 id nothing is found
        receivers[r].next_job = next_index >= 0 ? next_index : min_index;
    }

    state.force_schedule_update = false;
}
