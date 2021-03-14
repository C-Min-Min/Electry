import React from 'react';
import props from 'prop-types';
import './css/Dashboard.css'

import LiveConsummation from './LiveConsummation'
import Charts from './Chart'
import ConsumerDetails from './ConsumerDetails'
import LivePrice from './LivePrice'


const Dashboard = ({title}) => {
  title = "Dashboard"; 
  return(
    <div id='dashboard'>
      <div className='row' id='row-1'>
        <LiveConsummation />
        <Charts />
      </div>
      <div className='row' id='row-2'>
        <ConsumerDetails />
        <LivePrice />
      </div>
    </div>
  );
}

export default Dashboard