import React from 'react';
import { BrowserRouter, Route, Switch } from 'react-router-dom';

import {ThemeProvider} from "styled-components";
import { GlobalStyles } from "./components/Themes/globalStyles";
import { lightTheme, darkTheme } from "./components/Themes/Themes"
import  {useDarkMode} from "./components/Themes/useDarkMode"
import Toggle from "./components/Themes/Toggler"


import Title from './components/Title/Title'
import Navigaton from './components/Navigation/Navigation'
import useToken from './useToken';

import Login from './components/Login/Login';
import Dashboard from './components/Dashboard/Dashboard';
import Details from './components/Details/Details'
import History from './components/History/History'
import AmbientMode from './components/AmbientMode/AmbientMode'
import Settings from './components/Settings/Settings';

const App = () => {
  const { token, setToken } = useToken();

  const [theme, themeToggler, mountedComponent] = useDarkMode();

  const themeMode = theme === 'light' ? lightTheme : darkTheme;

  if(!token) {
    return <Login setToken={setToken} />
  }

  if(!mountedComponent) return <div/>

  return (
    <ThemeProvider theme={themeMode}>
      <>
        <GlobalStyles/>
          <div className="wrapper">
            <Toggle theme={theme} toggleTheme={themeToggler} />
            <Title />
            <Navigaton />
            <BrowserRouter>
                        <Switch>
                        <Route path="/dashboard">
                            <Dashboard />
                        </Route>
                        <Route path="/details">
                            <Details />
                        </Route>
                        <Route path="/history">
                            <History />
                        </Route>
                        <Route path="/ambient-mode">
                            <AmbientMode />
                        </Route>
                        <Route path="/settings">
                            <Settings />
                        </Route>
                        </Switch>
            </BrowserRouter>
            <h1>Application</h1>
          </div>
      </>
    </ThemeProvider>
  );
}

export default App;