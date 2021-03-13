import React from 'react';

const ConsumerDetails = ({consumerDetails}) => {
    return(
        <div className='consumerDetails box cube'>
            <p id='m_value' className='bigNumber'>6.47</p>
            <p id='currency' className='unit'>BGN</p>
        </div>
    );
}

export default ConsumerDetails;
