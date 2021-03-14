import React from 'react';
import {Line} from 'react-chartjs-2';
import data from '../../node-mysql/tables.json'



const state = {
  labels: ['00', '01', '02',
           '03', '04', '05', '06', '07',
           '08', '09','10', '11', '12',
           '13', '14', '15', '16', '17',
           '18', '19', '20', '21', '22',
           '23', '24'],
  datasets: [
    {
      label: 'Wh',
      fill: false,
      lineTension: 0.35,
      borderColor: 'rgba(0,0,0,1)',
      borderWidth: 2,
      data: data.map((mapper, i) => {
        return(mapper.power);
      }),
    }
  ]
}

const History = () => {
    return (
      <div className='history box'>
        <Line
            className='chart'
          data={state}
          options={{
            title:{
              display:false
            },
            legend:{
              display:false
            },
            responsive: true, 
            maintainAspectRatio: false, 
          }}
        />
      </div>
    );
  }

export default History;