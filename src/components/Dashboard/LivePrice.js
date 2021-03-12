import React, { useState } from 'react';

const LivePrice = ({livePrice}) => {
    return(
        <div className='livePrice box cube'>
            <p id='m_value' className='bigNumber'>6.47</p>
            <p id='currency' className='unit'>BGN</p>
        </div>
    );
}

export default LivePrice;