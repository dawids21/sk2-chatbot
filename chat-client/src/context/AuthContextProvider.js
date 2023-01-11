import React, { useEffect, useState } from "react";
import { useNavigate } from "react-router-dom";
import backend from "../config/backend";
import useSnackbar from "../hooks/use-snackbar";
import AuthContext from "./auth-context";

const AuthContextProvider = (props) => {
  const [id, setId] = useState(0);
  const [username, setUsername] = useState("");
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [token, setToken] = useState("");
  const alert = useSnackbar();
  const navigate = useNavigate();

  useEffect(() => {
    if (
      localStorage.getItem("auth-token") &&
      localStorage.getItem("auth-id") &&
      localStorage.getItem("auth-username")
    ) {
      const id = parseInt(localStorage.getItem("auth-id"));
      const username = localStorage.getItem("auth-username");
      const token = localStorage.getItem("auth-token");
      setId(id);
      setUsername(username);
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
    setId(data.user_id);
    setUsername(username);
    localStorage.setItem("auth-id", data.user_id);
    localStorage.setItem("auth-username", username);
    localStorage.setItem("auth-token", data.token);
    navigate("/");
  };

  const logoutHandler = () => {
    localStorage.removeItem("auth-id");
    localStorage.removeItem("auth-username");
    localStorage.removeItem("auth-token");
    setIsLoggedIn(false);
    setToken("");
    navigate("/login");
  };

  return (
    <AuthContext.Provider
      value={{
        isLoggedIn: isLoggedIn,
        token: token,
        id,
        username,
        onLogin: loginHandler,
        onLogout: logoutHandler,
      }}
    >
      {props.children}
    </AuthContext.Provider>
  );
};

export default AuthContextProvider;
