async function setLEDMode(mode) {
  await fetch('/api/led/' + mode, {method: "POST"})
}