import React from "react";

const AuthContext = React.createContext({
  token: "",
  isLoggedIn: false,
  onLogin: async (username, password) => {},
  onLogout: async () => {},
});

export default AuthContext;
