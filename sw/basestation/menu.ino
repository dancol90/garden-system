/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

Menu* root = new Menu(NULL, NULL);
Menu* timer_edit;

LcdDrawer dr = LcdDrawer(lcd);

Interval tmp_job; byte tmp_ind;

void init_menu() {
	Menu* sub;

    root->setText(F("Menu"));

    root->addItem(new Action(root, F("Forza accensione")));
    root->addItem(new Action(root, F("Forza spegnimento")));

    
    sub = new Menu(root, F("Timer"));

    for (uint8_t i = 0; i < 10; i++) {
        sub->addItem(new Action(sub, F("Job"), timer_selected, i));        
    }

    root->addItem(sub);


    timer_edit = new Menu(sub, F("Edit"));

        timer_edit->addItem(new TimeSelector(timer_edit, F("Ore"),    tmp_job.start));
        timer_edit->addItem(new TimeSelector(timer_edit, F("Minuti"), tmp_job.start));

        timer_edit->addItem(new Action(timer_edit, F("Salva"), timer_save));
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

// ############################################################################################
void timer_selected(int index) {
    tmp_job = receivers[state.selected_recv].jobs[index];
    tmp_ind = index;

    menu.takeControl(timer_edit);
}

void timer_save(int ignore) {
    receivers[state.selected_recv].jobs[tmp_ind] = tmp_job;

    // TODO: save in EEPROM

    print_jobs();
}