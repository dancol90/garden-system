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
    int8_t min_index = -1, next_index = -1;
    long   min_value =  0xFFFF;
    Interval job;

    // TODO: check all receivers, not just the current one

    // Before checking, set the current receiver as there are no jobs.
    // This will be overitten only if a job is actually found.
    // If not, it's ok for it to mantains this value.
    receivers[state.selected_recv].current_job = -1;

    for(byte i = 0; i < jobs_count; i++) {
        job = get_job(i);

        // The lowest one but after the current time
        if (job.enabled) {
            if (job.start.minutes < min_value) {
                // A smaller value than the last one was found. Save it
                min_value = job.start.minutes;
                min_index = i;

                // If this job is also later today, it's what we are searching for.
                if (job.start.minutes >= now.minutes)
                    next_index = i;
            }
            
            if (job.start.minutes <= now.minutes < job.end.minutes) {
                receivers[state.selected_recv].current_job = i;
            }
        }
    }

    // If no future jobs are found, return the absolute mininum
    // (that's the first of tomorrow) or -1 id nothing is found
    receivers[state.selected_recv].next_job = next_index >= 0 ? next_index : min_index;
}
