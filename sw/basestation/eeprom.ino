
Interval get_job(byte index) {
	Interval job;

	EEPROM.get(eeprom_jobs_start + sizeof(Interval) * index, job);

	return job;
}

void get_job_string(byte index, char* buff) {
	for (byte i = 0; i < 15; i++) {
		buff[i] = EEPROM.read(eeprom_strings_start + 15 * index + i);
	}

	buff[14] = 0x00;
}