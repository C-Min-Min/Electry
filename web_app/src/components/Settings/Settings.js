import React from 'react';
import { func, string } from 'prop-types';
import Toggle from "../Themes/Toggler"

const Settings = ({theme, themeToggler}) => {
  return(
    <div>
          <h2>Settings</h2>
      <Toggle className='tggl' theme={theme} toggleTheme={themeToggler} />
    </div>
  );
}

Settings.propTypes = {
  theme: string.isRequired,
  toggleTheme: func.isRequired,
}

export default Settings