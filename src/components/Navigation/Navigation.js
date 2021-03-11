import React from 'react';
import { BrowserRouter, Route, Switch } from 'react-router-dom';
import PropTypes from 'prop-types';

import './Navigation.css';


const Navigation = ({title}) => {
    return(
        <div className='navigation'>
            <h1>{title}</h1>
            <a href='/dashboard'>Home</a>
            <a href='/details'>Details</a>
            <a href='/history'>History</a>
            <a href='/ambient-mode'>Ambient mode</a>
            <a href='/settings'>Settings</a>
        </div>
        
    );
}

Navigation.propTypes = {
    title: PropTypes.string.isRequired
}

export default Navigation;