#define CONFIG_MAX 5            //< Maximum number of option levels
#define CONFIG_END -1           //< Sentinel for last option level

/**
 * @struct config_opt
 * A single configuration option with name, value, and setting choices
 */
typedef struct {
    char *name;                 //< User-readable option name
    int value;                  //< Current option value
    int levels[CONFIG_MAX];     //< Settable option levels
} config_opt;

/**
 * @struct config_opt_list
 * Option list for storing and passing configuration options
 */
typedef struct {
    config_opt *opts[CONFIG_MAX];
    int length;
} config_opt_list;

/**
 * Display and handle a configuration menu with the given options.
 */
void config_menu(config_opt_list options) {
    // Enable manual NXT button handling.
    nNxtButtonTask = -2;
    int opt_index = 0, sel_index = 0;

    while (true) {
        config_opt *curr_opt = options.opts[opt_index];
        int button;

        // Redraw configuration menu.
        eraseDisplay();
        displayTextLine(0, "%s < %d >", curr_opt->name,
                curr_opt->levels[sel_index]);

        // Wait for a button press.
        while (true) {
            button = nNxtButtonPressed;
            if (button != kNoButton) {
                break;
            }
            sleep(100);
        }

        switch (button) {
            case kLeftButton:
                // Make sure previous option level actually exists.
                if (sel_index - 1 >= 0) {
                    sel_index -= 1;
                }
                break;
            case kRightButton:
                // Make sure next option level actually exists.
                if (sel_index + 1 < CONFIG_MAX &&
                        curr_opt->levels[sel_index + 1] != CONFIG_END) {
                    sel_index += 1;
                }
                break;
            case kEnterButton:
                curr_opt->value = curr_opt->levels[sel_index];
                writeDebugStreamLine("[config_menu] %d selected for %s",
                        curr_opt->value, curr_opt->name);

                // Advance to next menu item.
                sel_index = 0;
                opt_index += 1;
                if (opt_index == options.length) {
                    eraseDisplay();
                    return;
                }
                break;
            default:
                break;
        }

        sleep(250);
    }
}
