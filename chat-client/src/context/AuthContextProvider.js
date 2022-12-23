import React, { useEffect, useState } from "react";
import { useNavigate } from "react-router-dom";
import backend from "../config/backend";
import useSnackbar from "../hooks/use-snackbar";
import AuthContext from "./auth-context";

const AuthContextProvider = (props) => {
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [token, setToken] = useState("");
  const alert = useSnackbar();
  const navigate = useNavigate();

  useEffect(() => {
    if (localStorage.getItem("auth-token")) {
      const token = localStorage.getItem("auth-token");
      setToken(token);
      setIsLoggedIn(true);
    }
  }, []);

  const loginHandler = async (username, password) => {
    const token = await fetch(`${backend.url}/login`, {
      method: "POST",
      body: JSON.stringify({ username, password }),
    });
    if (token.status !== 200) {
      alert("Wrong credentials", "error");
      return;
    }
    const data = await token.json();
    setToken(data.token);
    setIsLoggedIn(true);
    localStorage.setItem("auth-token", data.token);
    navigate("/");
  };

  const logoutHandler = () => {
    localStorage.removeItem("auth-token");
    setIsLoggedIn(false);
    setToken("");
    navigate("/login");
  };

  const getInfo = async () => {
    const response = await fetch(`${backend.url}/info/auth`, {
      method: "GET",
      headers: {
        Authorization: token,
      },
    });
    if (response.status !== 200) {
      return "User";
    }
    const data = await response.json();
    return data;
  };

  return (
    <AuthContext.Provider
      value={{
        isLoggedIn: isLoggedIn,
        token: token,
        onLogin: loginHandler,
        onLogout: logoutHandler,
        getInfo: getInfo,
      }}
    >
      {props.children}
    </AuthContext.Provider>
  );
};

export default AuthContextProvider;
