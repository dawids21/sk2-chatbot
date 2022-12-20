import React, { useState } from "react";
import { useNavigate } from "react-router-dom";
import backend from "../config/backend";
import useSnackbar from "../hooks/use-snackbar";
import AuthContext from "./auth-context";

const AuthContextProvider = (props) => {
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [token, setToken] = useState("");
  const alert = useSnackbar();
  const navigate = useNavigate();

  const loginHandler = async (username, password) => {
    const token = await fetch(`${backend.url}/login`, {
      method: "POST",
      body: JSON.stringify({ username, password }),
    });
    if (token.status !== 200) {
      alert("Wrong credentials", "error");
      return;
    }
    setIsLoggedIn(true);
    const data = await token.json();
    setToken(data.token);
    navigate("/");
  };

  const logoutHandler = () => {
    setIsLoggedIn(false);
    setToken("");
    navigate("/login");
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
