import React, { useState } from 'react';

const LiveConsummation = ({liveConsummation}) => {
    return(
        <div className='liveConsummation  box cube'>
            <p id='consummationValue' className='bigNumber'>1.47</p>
            <p id='consummationMesurement' className='unit'>kWh</p>
        </div>
    );
}

export default LiveConsummation;