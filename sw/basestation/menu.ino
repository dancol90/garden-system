/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

Menu* root = new Menu(NULL, NULL);
Menu* timer_edit;
MenuItem* add_entry;

LcdDrawer dr = LcdDrawer(lcd);

Interval job_copy; uint8_t job_ind;

void init_menu() {

	Menu* sub;

    root->setText(F("Menu"));

    root->addItem(new Action(root, F("Forza accensione"),  force_recv_on));
    root->addItem(new Action(root, F("Forza spegnimento"), force_recv_off));
   
    sub = new Menu(root, F("Timer"));

    for (uint8_t i = 0; i < jobs_count; i++) {
        sub->addItem(new JobEntry(sub, job_selected, i));        
    }

    add_entry = sub->addItem(new Action(sub, F("Aggiungi"), job_add));

    root->addItem(sub);
    
    timer_edit = new Menu(sub, F("Edit"));

        timer_edit->addItem(new TimeSelector(timer_edit, F("Accendi alle"), job_copy.start, job_changed));
        timer_edit->addItem(new TimeSelector(timer_edit, F("Spegni alle"),  job_copy.end,   job_changed));

        timer_edit->addItem(new Action(timer_edit, F("Elimina"), job_delete));

    sub = new Menu(root, F("Impostazioni"));

        sub->addItem(new NumericSelector(sub, F("Retroilluminazione"), settings.backlight, 1, 20, lcd_cb));
        sub->addItem(new NumericSelector(sub, F("Contrasto"),          settings.contrast,  1, 20, lcd_cb));
        sub->addItem(new    TimeSelector(sub, F("Ora"),                now, time_cb));


        sub->addItem(new          Action(sub, F("Ripristina memoria"), factory_wipe));    

        root->addItem(sub);

    menu = MenuController(root, &dr);
}

void update_menu() {
    bool changed = true;

    if (is_pressed(BTN_UP))
        menu.prev();
    else if (is_pressed(BTN_DOWN))
        menu.next();
    else if (is_pressed(BTN_OK))
        menu.select();
    else if (is_pressed(BTN_BACK)) {
        menu.back();
        if (menu.canExit()) state.menu_active = false;
    } else
        changed = false;

    if (changed)
        menu.draw();
}

void enter_menu() {
    state.menu_active = true;
    menu.takeControl(root);

    menu.draw();
}

// ############################################################################################

void job_add() {
    // For each job of the current receiver
    for(byte i = 0; i < jobs_count; i++) {
        // If it's not enabled
        if (!get_job(i).enabled) {
            // Select it
            job_selected(i);

            return;
        }
    }
}

void job_selected(uint8_t index) {
    // Save a copy to work with
    job_copy = get_job(index);
    // Save the ref for future save
    job_ind = index;

    menu.takeControl(timer_edit);
}

void job_update() {
    // Write to EEPROM
    save_job(job_ind, job_copy);

    // Update "Add job" menu entry to reflect available space
    bool avail = false;

    for (byte i = 0; i < jobs_count; i++) {
        if (!get_job(i).enabled) {
            avail = true;
            break;
        }
    }

    add_entry->setState(avail);

    state.force_schedule_update = true;
}

/*
    Called when either start or stop time of a job changes.
    It does some validation, so that a start time that comes later than the end time
    is not accepted and saved.

    TODO: manage 0:00 rollover
*/
void job_changed(bool enter) {
    // Update on exit
    if (!enter && job_copy.start.minutes < job_copy.end.minutes) {
        job_copy.enabled = true;
        job_update();
    }
}

void job_delete() {
    memset(&job_copy, 0, sizeof(Interval));
    job_update();
    menu.back();
}

// ############################################################################################

void force_recv(uint8_t s) {
    selected_receiver.active = s;
    selected_receiver.forced = true;

    state.menu_active = false;
}
void force_recv_on()  { force_recv(true);  }
void force_recv_off() { force_recv(false); }

// ############################################################################################

// Update both contrast & backlight when one of them changes
void lcd_cb(bool confirm) {
    state.lcd_settings_dirty = true;

    if (confirm) 
        save_settings();
}

void time_cb(bool enter) {
    if (enter) {
        // The selector was just entered
        // Stop updating the time while modifing it.
        state.rtc_stop = true;
    } else {
        // Set RTC time here.
        write_time(now);
        
        state.rtc_stop = false;
    }
}

void factory_wipe() {
    format_eeprom();

    // TODO: restore other settings

    state.menu_active = false;
}
