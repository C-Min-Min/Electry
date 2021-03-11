import React from 'react';
import { BrowserRouter, Route, Switch } from 'react-router-dom';



const Navigation = () => {
    return(
        <div className='navigation'>
            <a href='/dashboard'>Home</a>
            <a href='/details'>Details</a>
            <a href='/history'>History</a>
            <a href='/ambient-mode'>Ambient mode</a>
            <a href='/settings'>Settings</a>

        </div>
        
    );
}

export default Navigation;