import React from 'react';
import { CircularProgressbar } from 'react-circular-progressbar';

const LiveConsummation = ({liveConsummation}) => {
    const value = 1440;
    return(
        <div className='liveConsummation  box cube'>
            <CircularProgressbar value={value} maxValue={5000} text={`${value * 100}%`} />;
            <p id='consummationMesurement' className='unit'>kWh</p>
        </div>
    );
}

export default LiveConsummation;