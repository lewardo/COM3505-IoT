let chart;

let CURRENT_TEMP = document.getElementById("currentTemp");
let CONNECTION_STATUS = document.getElementById("connectionStatus");

async function fetchData() {
    const response = await fetch('/api/data');
    const data = await response.json();

    const filtered = data.filter(d => d.temperature !== undefined);
    const values = filtered.map(d => d.temperature);
    const labels = filtered.map(d => d.label);

    if (!chart) {
        // Create new graph on first load
        const ctx = document.getElementById('temperatureChart');
        chart = new Chart(ctx, {
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
                    }
                },
                animation: {
                    onComplete: function () {
                        document.getElementById("loadingSpinner").style.display = "none";
                        document.getElementById("mainContent").style.display = "block";
                    }
                }
            }
        });
    } else {
        // Update graph
        chart.data.labels = labels;
        chart.data.datasets[0].data = values;
        chart.update();
    }

    // Update current temp value
    let recent = values[values.length - 1];
    if (recent != undefined) {
        CURRENT_TEMP.textContent = "Current Temperature: " + recent + "℃";
    }

    // Update connection status upon receiving data
    if (data.length > 0 && CONNECTION_STATUS.textContent != "Connected") {
        CONNECTION_STATUS.textContent = "Connected";
        CONNECTION_STATUS.classList.remove("bg-danger");
        CONNECTION_STATUS.classList.add("bg-success");
    }
}

setInterval(fetchData, 2000);
