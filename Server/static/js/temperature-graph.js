let chart;

async function fetchData() {
    const response = await fetch('/api/data');
    const data = await response.json();

    const labels = data.map((_, i) => i);
    const values = data.map(d => d.temperature);

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
                }
            }
        });
    } else {
        chart.data.labels = labels;
        chart.data.datasets[0].data = values;
        chart.update();
    }
}

setInterval(fetchData, 2000);