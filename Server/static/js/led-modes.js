// Constants //////////////////////////////////////////////
const LED_COUNT = 12;
const MODES_NAMES = {
    'b': "Blink",
    'w': "Twinkle",
    'l': "Chase - Linear",
    'i': "Chase - Ease In",
    'o': "Chase - Ease Out",
    'x': "Chase - Ease In & Out",
    'r': "Rainbow",
    'f': "Flame",
    'c': "Binary Counter",
    't': "Thermometer",
    'm': "Manual"
}

// Default Values /////////////////////////////////////////
let delay = 500;
let mode = 'b';
let pattern = 0;
let ledStates = [];

// HTML elements //////////////////////////////////////////
const DELAY_SLIDER = document.getElementById('delay');
const LED_FIELD = document.getElementById('led-field');
const DELAY_OUTPUT = document.getElementById('delayValue');
const CURRENT_MODE = document.getElementById('currentMode');

// LED modes //////////////////////////////////////////////
function setLEDMode(new_mode, new_pattern = null) {
    // Pressing manual button repeatedly shouldn't clear existing pattern
    if (mode === 'm' && new_mode === 'm' & new_pattern === pattern) return

    // Otherwise update modes
    mode = new_mode;
    if (new_pattern != null) {
        pattern = new_pattern;
    }
    fetch('/api/led', {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({
            mode: mode,
            pattern: pattern,
            delay: delay
        })
    });

    // Show appropriate content depending on mode
    CURRENT_MODE.textContent =`Current LED Pattern: ${MODES_NAMES[mode]}`;
    DELAY_SLIDER.hidden = mode == 'm' || mode == 't';
    DELAY_OUTPUT.hidden = mode == 'm' || mode == 't';
    LED_FIELD.hidden = mode != 'm';
}

// Delay Slider ///////////////////////////////////////////
// Update HTML value constantly
DELAY_SLIDER.addEventListener('input', function() {
    delay = parseInt(this.value);
    DELAY_OUTPUT.textContent = "Delay: " + this.value;
});

// Only POST delay value when slider stops moving
DELAY_SLIDER.addEventListener('change', function() {
    setLEDMode(mode);
});

// Manual LED mode ////////////////////////////////////////
document.querySelectorAll('.led').forEach((led, index) => {
    const color = led.dataset.color;
    led.src = document.getElementById(`preload-${color}OFF`).src;

    led.addEventListener('click', () => {
        // On Click, invert that LED's state
        const isOff = led.dataset.state === 'OFF';
        const new_state = isOff ? 'ON' : 'OFF';

        ledStates[index] = isOff;
        setLEDMode('m', statesToValue());

        led.src = document.getElementById(`preload-${color}${new_state}`).src;
        led.dataset.state = new_state;
        led.classList.toggle(`on-${color}`, isOff);
    });
});

// Convert the ledStates array into an integer representation value
function statesToValue() {
    let value = 0;
    ledStates.forEach((state, index) => {
        let power = LED_COUNT - index - 1;
        value += state * 2 ** power;
    });
    return value
}
