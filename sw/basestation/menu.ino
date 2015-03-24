/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

Menu* root = new Menu(NULL, NULL);
Menu* timer_edit;

NokiaLcdDrawer dr = NokiaLcdDrawer(lcd);

void init_menu() {
	Menu* sub;

    root->setText(F("Menu"));

    root->addItem(new Action(root, F("Forza accensione")));
    root->addItem(new Action(root, F("Forza spegnimento")));

    sub = new Menu(root, F("Timer"), &timer_list_entered);

        sub->addItem(new Action(sub, F("Aggiungi"), timer_selected, 1));
        sub->addItem(new Action(sub, F("15:00 -> 16:10"), timer_selected, 2));

        root->addItem(sub);


    timer_edit = new Menu(sub, F("Edit"));

    sub = new Menu(root, F("Impostazioni"));

        sub->addItem(new NumericSelector(sub, F("Retroilluminazione"), state.backlight, 1, 10));
        sub->addItem(new NumericSelector(sub, F("Contrasto"), state.contrast, 1, 10));

        root->addItem(sub);


    // timer_edit->addItem(new NumericSelector(timer_edit, F("Ore"), h, 0, 24));
    // timer_edit->addItem(new NumericSelector(timer_edit, F("Minuti"), m, 0, 60));
    // timer_edit->addItem(new Action(timer_edit, F("Elimina")));

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

void timer_list_entered(Menu* menu) {
    return;
}

void timer_selected(int index) {
    menu.takeControl(timer_edit);
}