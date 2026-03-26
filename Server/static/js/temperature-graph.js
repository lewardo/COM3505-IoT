let chart;

async function fetchData() {
    const response = await fetch('/api/data');
    const data = await response.json();

    const filtered = data.filter(d => d.temperature !== undefined);
    const values = filtered.map(d => d.temperature);
    const labels = filtered.map(d => d.label);

    if (!chart) {
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
                        beginAtZero: true
                    }
                },
                animation: {
                    onComplete: function () {
                    showContent();
                    }
                }
            }
        });
    } else {
        chart.data.labels = labels;
        chart.data.datasets[0].data = values;
        chart.update();
    }

    let recent = values[values.length - 1];
    if (recent != undefined) {
        document.getElementById("currentTemp").innerHTML = "Current Temperature: " + recent + "&#8451";
    }

    if (data.length > 0) {
        let status = document.getElementById("connectionStatus");
        status.innerHTML = "Connected";
        status.classList.remove("bg-danger");
        status.classList.add("bg-success");
    }
}

setInterval(fetchData, 2000);

function showContent() {
    document.getElementById("loadingSpinner").style.display = "none";
    document.getElementById("mainContent").style.display = "block";
}
