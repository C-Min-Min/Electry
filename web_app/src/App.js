import React, { useState } from 'react';
import { BrowserRouter, Route, Switch } from 'react-router-dom';
import { Redirect } from 'react-router';

import {ThemeProvider} from "styled-components";
import { GlobalStyles } from "./components/Themes/globalStyles";
import { lightTheme, darkTheme } from "./components/Themes/Themes"
import  {useDarkMode} from "./components/Themes/useDarkMode"

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

  let [title] = "Dashboard";

  const themeMode = theme === 'light' ? lightTheme : darkTheme;

  if(!token) {
    return(
      <Login setToken={setToken} />
    );
  }

  if(!mountedComponent) return <div/>

  return (
    <ThemeProvider theme={themeMode}>
      <>
        <GlobalStyles/>
          <div className="wrapper">
            <Title />
            <Navigaton className='nav' title={title}/>
            <BrowserRouter>
                <Switch>
                <Route path="/dashboard">
                    <Dashboard title={title}/>
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
                    <Settings theme={theme} themeToggler={themeToggler}/>
                </Route>
                <Route exact path="/">
                    <Redirect to="/dashboard" />
                </Route>
                </Switch>
            </BrowserRouter>
          </div>
      </>
    </ThemeProvider>
  );
}

export default App;