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
    Serial.println(free_ram());

	Menu* sub;

    root->setText(F("Menu"));

    root->addItem(new Action<byte>(root, F("Forza accensione"),  force_recv, 1));
    root->addItem(new Action<byte>(root, F("Forza spegnimento"), force_recv, 0));
    
    Serial.println(free_ram());

    
    sub = new Menu(root, F("Timer"));

    for (uint8_t i = 0; i < 5; i++) {
        sub->addItem(new JobEntry(sub, job_selected, i));        
    }

    sub->addItem(new Action<byte>(sub, F("Aggiungi"), job_add, NULL));

    root->addItem(sub);

    Serial.println(free_ram());
    
    timer_edit = new Menu(sub, F("Edit"));

        timer_edit->addItem(new TimeSelector(timer_edit, F("Accendi alle"), job_copy.start));
        timer_edit->addItem(new TimeSelector(timer_edit, F("Spegni alle"),  job_copy.end));

        timer_edit->addItem(new Action<uint8_t>(timer_edit, F("Salva"), job_save, 0));
        //timer_edit->addItem(new Action(timer_edit, F("Disabilita")));

    Serial.println(free_ram());

    sub = new Menu(root, F("Impostazioni"));

        sub->addItem(new NumericSelector(sub, F("Retroilluminazione"), state.backlight, 1, 10));
        sub->addItem(new NumericSelector(sub, F("Contrasto"), state.contrast, 1, 10));

        root->addItem(sub);

    menu = MenuController(root, &dr);

    Serial.println(free_ram());
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

    Serial.println(free_ram());
}

// ############################################################################################

void job_add(uint8_t i) {
    // For each job of the current receiver
    for(i = 0; i < 10; i++) {
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

void job_save(uint8_t ignore) {
    print_job(job_copy);
    //set_job(job_ind, job_copy);

    // TODO: disable "Aggiungi" item if all jobs are enabled
    // TODO: save in EEPROM

    menu.back();
}

void force_recv(uint8_t s) {
    selected_receiver.active = s;

    state.menu_active = false;
}