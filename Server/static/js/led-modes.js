const LED_COUNT = 12;

// Default Values
let delay = 500;
let mode = 'm';
let pattern = 0;
let ledStates = [];

// HTML elements
const DELAY_SLIDER = document.getElementById('delay');
const LED_FIELD = document.getElementById('led-field');
const DELAY_OUTPUT = document.getElementById('delayValue');

// LED modes //////////////////////////////////////////////
function setLEDMode(new_mode, new_pattern = null) {
  // Pressing manual button again shouldn't clear existing pattern
  if (mode === 'm' && new_mode === 'm' & new_pattern === pattern) return
  
  // Otherwise update modes
  mode = new_mode;
  if (new_pattern != null) {
    pattern = new_pattern;
  }
  
  fetch('/api/led/', {
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
  manual_mode = mode == 'm';
  DELAY_SLIDER.hidden = manual_mode;
  DELAY_OUTPUT.hidden = manual_mode;
  LED_FIELD.hidden = !manual_mode;
}

// Delay Slider ///////////////////////////////////////////
DELAY_SLIDER.addEventListener('input', function() {
  delay = this.value;
  DELAY_OUTPUT.textContent = "Delay: " + this.value;
});

DELAY_SLIDER.addEventListener('change', function() {
  setLEDMode(mode);
});

// Manual LED mode ////////////////////////////////////////
document.querySelectorAll('.led').forEach((led, index) => {
  const color = led.dataset.color;
  led.src = document.getElementById(`preload-${color}OFF`).src;

  led.addEventListener('click', () => {
    const isOff = led.dataset.state === 'OFF';
    const new_state = isOff ? 'ON' : 'OFF';

    ledStates[index] = isOff;
    setLEDMode('m', statesToValue());

    led.src = document.getElementById(`preload-${color}${new_state}`).src;
    led.dataset.state = new_state;
    led.classList.toggle(`on-${color}`, isOff);
  });
});

function statesToValue() {
  let value = 0;
  ledStates.forEach((state, index) => {
    let power = LED_COUNT - index - 1;
    value += state * 2**power;
  });
  return value
}
