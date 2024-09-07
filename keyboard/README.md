# Keyboard Firmware with QMK

## TOC
1. [Programming in QMK](#1-programming-in-qmk)
2. [How the QMK framework works](#2-how-the-qmk-firmware-works)
3. [Add new keycodes](#3-add-new-keycodes)

## Some Interesting Links
- A 3-part article on starting with QMK, from simple things to very complex stuff ([1](https://getreuer.info/posts/keyboards/macros/index.html), [2](https://getreuer.info/posts/keyboards/triggers/index.html), [3](https://getreuer.info/posts/keyboards/macros3/index.html)) 
- How the QMK firmware works ([doc](https://docs.qmk.fm/understanding_qmk))

## 1. Programming in QMK
To program with QMK, you need to follow these steps:
1. Find your keyboard in the `/keyboards` folder;
2. Go in the `/keymaps` directory;
3. Make a new directory for your custom configuration;
4. Copy the content of the `/keymaps/default` directory into your new directory;
5. Open the `keymap.c` file and start customizing!

The `keymap.c` files always contains the _layers_ of the keyboard. For example, in the Planck default keymap, we have:
```c
// definition for the keymap names/indices
enum planck_layers {
  _QWERTY,  // 0
  _COLEMAK, // 1
  _DVORAK,  // 2
  _LOWER,   // 3
  _RAISE,   // 4
  _PLOVER,  // 5
  _ADJUST   // 6
};
// layer matrix
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Brite| Ctrl | Alt  | GUI  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = LAYOUT_planck_grid(
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
    KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT ,
    BACKLIT, KC_LCTL, KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
),
// [...]
}
```
The layer at index 0 is the default layer. 

Layers can be separated in base layers and function layers. Base layers are QWERTY, Colemak, and Dvorak, in this example. Function layers are placed on top of the base layer, to change its behavior. In this case, raise, lower and adjust are function layers. If a key doesn't change between base and function layer, it should be represented with a transparent keycode `_______`.

It's possible to remove, add and modify layers. The important thing is to keep the `enum` consistent with the changes. For example, if I want Colemak to be the default layer, I can swap it with QWERTY in both `keymaps` and `enum`:
```c
enum planck_layers {
  _COLEMAK, // 0
  _QWERTY,  // 1
  _DVORAK,  // 2
  // [...]
};
```

To further customize your keyboard, you have to use _callback functions_. Callback functions in QMK always end by **\_kb** (at keyboard level) or **\_user** (at keycode level). `_user` functions are empty, and are always called by the respective `_kb` function. `_kb` functions can be already defined -- or not, so double check if you plan on using it.

Usually, you'll put your changes in the `_user` version. If you choose to change the `_kb` version, make sure to call the `_user` version in it, and remember you'll override the original one (so take a look at what it does).

A list of functions and callbacks can be found in the `.h` files in the [`/quantum` folder](https://github.com/qmk/qmk_firmware/blob/master/quantum/).

To see an example on how to define a new keycode, see [chapter 3](#3-add-new-keycodes).

## 2. How the QMK firmware works
_Inspired by [this document](https://docs.qmk.fm/understanding_qmk)_

This is how the [main function of QMK](https://github.com/qmk/qmk_firmware/blob/0.15.13/quantum/main.c#L55) works:
```c
int main(void) {
    platform_setup();
    protocol_setup();
    keyboard_setup();

    protocol_init();

    /* Main loop */
    while (true) {
        protocol_task();
        housekeeping_task();
    }
}
```
Let's focus on the main loop. This loop is the most important part of the firmware, and it continues forever, occupying the CPU of the microcontroller for 99% of the time.

For each iteration of the main loop, the firmware runs two functions:
1. `protocol_task` : this function is responsible for scanning the pin matrix, detecting changes in it, and calling `process_record` for each change. It also handles LEDs, and can manipulate the mouse.
2. `housekeeping_task` : this function runs some code every time the loop is finished. By default, it's empty.

### `process_record`
This is the function responsible to process a change in the pin matrix. According to the options, `process_record` triggers a chain of multiple functions. You can find a full list of them [here](https://docs.qmk.fm/understanding_qmk#process-record). 

A more simplified list of them is the following:
```
main
`- protocol_task
   `- keyboard_task
      |- matrix_scan
      |  // for each change in the scan
      `- action_exec
         |- pre_process_record_kb
         |  `- pre_process_record_user
         |  // START process_record chain, stop when something return `false`
         |- process_record_kb
         |  `- process_record_user
         |- <default process functions>
         |  // END process_record chain
         `- post_process_record_kb
            `- post_process_record_user
```

In general, the chain will be halted as soon as a function returns `false`. After the chain is stopped, `post_process_record` is called to perform extra cleanup.

## 3. Add new keycodes
After changing the keyboard layers, the most common customization is to change/add keycodes. This is very easy to do!

Suppose you want to add a keycode that writes "../". First thing first, you need to assign a constant value to this keycode. This value should be different from any other keycode, otherwise there will be ambiguity. This can be done with the `SAFE_RANGE` macro.
```c
enum new_keycodes {
    KC_PARENT_DIR = SAFE_RANGE
    // you can add other keycodes here
    // if you do so, there is no need to write SAFE_RANGE again
}
```
Then, you need to define what must happen when you press/release the keycode. If you read the rest of this document, you'll know that the event of pressing/releasing a keycode will be handled by `process_record`, and that `process_record_user` is the callback function reserved to add custom code. You'll also know that returning `false` means "disregard any default behavior", since you'll break the processing chain.

So, time to override! The `process_record_user` function should look like this:
```c
bool process_record_user(uint16_t keycode, keyrecord_t *record);
```
where `keyrecord_t` is a structure that looks like this:
```c
keyrecord_t record {
    keyevent_t event {
        keypos_t key {
            uint8_t col
            uint8_t row
        }
        bool     pressed
        uint16_t time
    }
}
```
Overall, `process_record_user` will be called in the main loop for each change in the pin matrix. `keycode` will store the keycode that has changed, while `record` will provide the information on the change (are we pressing the key, or releasing it? And what's the position of the key in the matrix?).

Now, you have everything you need! You simply need to add the following code in `keymap.c`:
```c
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case KC_PARENT_DIR:
            // we are typing on pressing, and not on releasing
            if (record->event.pressed) {
                SEND_STRING("../");
            }
            // there is no default behavior for this keycode, stop the chain
            return false;
        default:
            // continue the chain
            return true;
    }
}
```