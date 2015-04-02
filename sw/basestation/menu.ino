/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

Menu* root = new Menu(NULL, NULL);
Menu* timer_edit;

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

    sub->addItem(new Action(sub, F("Aggiungi"), job_add));

    root->addItem(sub);
    
    timer_edit = new Menu(sub, F("Edit"));

        timer_edit->addItem(new TimeSelector(timer_edit, F("Accendi alle"), job_copy.start));
        timer_edit->addItem(new TimeSelector(timer_edit, F("Spegni alle"),  job_copy.end));

        timer_edit->addItem(new Action(timer_edit, F("Salva"), job_save));
        timer_edit->addItem(new Action(timer_edit, F("Disabilita"), job_disable));

    sub = new Menu(root, F("Impostazioni"));

        sub->addItem(new NumericSelector(sub, F("Retroilluminazione"), settings.backlight, 1, 10, lcd_cb));
        sub->addItem(new NumericSelector(sub, F("Contrasto"),          settings.contrast,  1, 10, lcd_cb));

        sub->addItem(new Action(root, F("Ripristina memoria"), factory_wipe));    

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

void job_save() {
    save_job(job_ind, job_copy);

    // TODO: disable "Aggiungi" item if all jobs are enabled

    menu.back();
}

void job_disable() {

}

void force_recv(uint8_t s) {
    selected_receiver.active = s;

    state.menu_active = false;
}
void force_recv_on()  { force_recv(true);  }
void force_recv_off() { force_recv(false); }

void factory_wipe() {
    format_eeprom();

    // TODO: restore other settings

    state.menu_active = false;
}