import React from 'react';
import PropTypes from 'prop-types';

import './Navigation.css';
import logo from '../../img/logo.png'


const Navigation = ({title}) => {
    return(
        <div className='navigation'>
            <div className='innerNav'>
                <h1>{title}</h1>
                <a href='/dashboard'>Dashboard</a>
                <a href='/details'>Details</a>
                <a href='/history'>History</a>
                <a href='/ambient-mode'>Ambient mode</a>
                <a href='/settings'>Settings</a>
            </div>
            <img id='logo' src={logo}/>
        </div>
        
    );
}

Navigation.propTypes = {
    title:PropTypes.string.isRequired
}

export default Navigation;