import React from 'react';
import { CircularProgressbar } from 'react-circular-progressbar';


const ConsumerDetails = ({consumerDetails}) => {
    return(
        <div className='consumerDetails box'>
            <div className='image-wrapper box'>
                <img src='https://www.pngrepo.com/download/58815/oven.png'/>
            </div>
            <div>
                <h3>Oven</h3>
                <ul>
                    <li>Description: <span className='value'>This is information. This is information. This is information. This is information. This is information. This is information. This is information. This is information. This is information. This is information. This is information. This is information. This is information. This is information. This is information. This is information. This is information. </span></li>
                    <li>RT Consumation: <span className='value'>XX</span>wH</li>
                    <li>Consumation (1H): <span className='value'>XX</span>wH</li>
                    <li>Consumation (24H): <span className='value'>XX</span>wH</li>
                </ul>
            </div>
        </div>
    );
}

export default ConsumerDetails;
