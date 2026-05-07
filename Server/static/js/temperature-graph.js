// Constants //////////////////////////////////////////////
const CURRENT_TEMP = document.getElementById("currentTemp");
const CONNECTION_STATUS = document.getElementById("connectionStatus");
const CTX = document.getElementById('temperatureChart');

// Default Values /////////////////////////////////////////
let chart;

// Temperature Graph //////////////////////////////////////
/**
 * Fetches the newest temperature data from the Flask server.
 */
async function fetchData() {
    const response = await fetch('/api/data');
    const data = await response.json();

    // Filter data to only labelled temperature values
    const filtered = data.filter(d => d.temperature !== undefined);
    const values = filtered.map(d => d.temperature);
    const labels = filtered.map(d => d.label);

    if (!chart) {
        // Create new graph on first load
        chart = new Chart(CTX, {
            type: 'line',
            data: {
                labels: labels,
                datasets: [{
                    label: 'Temperature',
                    data: values
                }]
            },
            options: {
                scales: {
                    y: {
                        min: 0,
                        suggestedMax: 30,
                        title: {
                            display: true,
                            text: 'Temperature'
                        }
                    },
                    x: {
                        title: {
                            display: true,
                            text: 'Reading'
                        }
                    }
                },
                animation: {
                    onComplete: function() {
                        document.getElementById("loadingSpinner").style.display = "none";
                        document.getElementById("mainContent").style.display = "block";
                    }
                }
            }
        });

    } else {
        // Update graph on subsequent calls
        chart.data.labels = labels;
        chart.data.datasets[0].data = values;
        chart.update();
    }

    // Update on-screen temperature value
    let recent = values[values.length - 1];
    if (recent != undefined) {
        CURRENT_TEMP.textContent = "Current Temperature: " + recent + "℃";
    }

    // Update on-screen connection status upon receiving data
    if (data.length > 0 && CONNECTION_STATUS.textContent != "Connected") {
        CONNECTION_STATUS.textContent = "Connected";
        CONNECTION_STATUS.classList.remove("bg-danger");
        CONNECTION_STATUS.classList.add("bg-success");
    }
}

// Request new data every 2 seconds
setInterval(fetchData, 2000);
