import React from 'react';
import props from 'prop-types';
import './css/Dashboard.css'

import LiveConsummation from './LiveConsummation'
import History from './History'
import ConsumerDetails from './ConsumerDetails'
import LivePrice from './LivePrice'


const Dashboard = ({title}) => {
  title = "Dashboard"; 
  return(
    <div id='dashboard'>
      <LiveConsummation />
      <History />
      <ConsumerDetails />
      <LivePrice />
    </div>
  );
}

export default Dashboard