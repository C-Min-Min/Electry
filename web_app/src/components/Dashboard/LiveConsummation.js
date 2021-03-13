import React from 'react';
import { CircularProgressbar } from 'react-circular-progressbar';

const LiveConsummation = ({liveConsummation}) => {
    const value = 1440;
    return(
        <div className='liveConsummation  box cube'>
            <p id='m_value' className='bigNumber'>6.47</p>
            <p id='consummationMesurement' className='unit'>kWh</p>
        </div>
    );
}

export default LiveConsummation;