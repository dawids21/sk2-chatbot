import React, { useState } from "react";
import AuthContext from "./auth-context";

const AuthContextProvider = (props) => {
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [token, setToken] = useState("");

  const loginHandler = (email, password) => {
    setIsLoggedIn(true);
    setToken("12345");
  };

  const logoutHandler = () => {
    setIsLoggedIn(false);
    setToken("");
  };

  return (
    <AuthContext.Provider
      value={{
        isLoggedIn: isLoggedIn,
        token: token,
        onLogin: loginHandler,
        onLogout: logoutHandler,
      }}
    >
      {props.children}
    </AuthContext.Provider>
  );
};

export default AuthContextProvider;
