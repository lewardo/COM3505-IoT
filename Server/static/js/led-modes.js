let delay = 500;
let mode = 'b';

async function setLEDMode(new_mode) {
  fetch('/api/led', {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify({
      mode: new_mode,
      delay: delay
    })
  });
  mode = new_mode;
  console.log("Set to: " + mode + ";0;" + delay);

  if (mode == 't' || mode == 'm') {
    document.getElementById('delay').disabled = true;
  } else {
    document.getElementById('delay').removeAttribute('disabled');
  }
}

const rangeInput = document.getElementById('delay');
const rangeOutput = document.getElementById('delayValue');

rangeInput.addEventListener('input', function() {
  delay = this.value;
  rangeOutput.textContent = "Delay: " + this.value;
});

rangeInput.addEventListener('change', function() {
  delay = this.value;
  setLEDMode(mode);
});
