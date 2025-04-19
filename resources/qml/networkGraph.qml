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
      labelsFont.pointSize: 8
      min: 0
      max: 60
    }

    ValueAxis {
      id: yAxis
      labelsFont.pointSize: 8
      min: 0
      max: 500 * 1024 // in bytes/sec, 500kiB/s
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
