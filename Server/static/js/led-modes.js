async function setLEDMode(mode) {
  const response = await fetch('/api/led/' + mode, {method: "POST"})
}