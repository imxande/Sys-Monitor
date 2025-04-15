import QtQuick
import QtQuick.Controls
import QtCharts

Item {
  width: 600
  height: 300

  ChartView {
    id: netChart
    anchors.fill: parent
    antialiasing: true
    theme: ChartView.ChartThemeDark

    ValueAxis {
      id: xAxis
      min: 0
      max: 60
      titleText: "Time (s)"
    }

    ValueAxis {
      id: yAxis
      min: 0
      max: 500 * 1024 // in bytes/sec, 500kiB/s
      titleText: "Network Rate (bytes/s)"
    }

    LineSeries {
      id: rxSeries
      name: "Receiving"
      axisX: xAxis
      axisY: yAxis
      color: "dodgerblue"
    }

    LineSeries {
      id: txSeries  
      name: "Sending"
      axisX: xAxis
      axisY: yAxis
      color: "orangered"
    }    

    Timer {
      interval: 1000
      running: true
      repeat: true
      onTriggered: {
        let rxVal = resourceMonitor.rxRate || 0
        let txVal = resourceMonitor.txRate || 0

        if (rxSeries.count > 60) rxSeries.remove(0)
        if (txSeries.count > 60) txSeries.remove(0)

        rxSeries.append(rxSeries.count, rxVal)
        txSeries.append(txSeries.count, txVal)
      }
    }
  }
}
