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

    root->addItem(new Action<uint8_t>(root, F("Forza accensione"),  force_recv, 1));
    root->addItem(new Action<uint8_t>(root, F("Forza spegnimento"), force_recv, 0));

    
    sub = new Menu(root, F("Timer"));

    for (uint8_t i = 0; i < 10; i++) {
        sub->addItem(new JobEntry(sub, timer_selected, i));        
    }

    root->addItem(sub);


    timer_edit = new Menu(sub, F("Edit"));

        timer_edit->addItem(new TimeSelector(timer_edit, F("Accendi alle"), job_copy.start));
        timer_edit->addItem(new TimeSelector(timer_edit, F("Spegni alle"),  job_copy.end));

        timer_edit->addItem(new Action<uint8_t>(timer_edit, F("Salva"), timer_save, 0));
        //timer_edit->addItem(new Action(timer_edit, F("Disabilita")));

    sub = new Menu(root, F("Impostazioni"));

        sub->addItem(new NumericSelector(sub, F("Retroilluminazione"), state.backlight, 1, 10));
        sub->addItem(new NumericSelector(sub, F("Contrasto"), state.contrast, 1, 10));

        root->addItem(sub);

    menu = MenuController(root, &dr);
}

void update_menu() {
    if (is_pressed(BTN_UP))
        menu.prev();
    else if (is_pressed(BTN_DOWN))
        menu.next();
    else if (is_pressed(BTN_OK))
        menu.select();
    else if (is_pressed(BTN_BACK)) {
        menu.back();
        if (menu.canExit()) state.menu_active = false;
    }

    menu.draw();
}

void enter_menu() {
    state.menu_active = true;
    menu.takeControl(root);
}

// ############################################################################################
void timer_selected(uint8_t index) {

    // Save a copy to work with
    job_copy = get_job(index);
    // Save the ref for future save
    job_ind = index;

    menu.takeControl(timer_edit);
}

void timer_save(uint8_t ignore) {
    set_job(job_ind, job_copy);

    // TODO: save in EEPROM
}


void force_recv(uint8_t state) {
    selected_recv.active = state;
}