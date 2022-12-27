import React from "react";

const AuthContext = React.createContext({
  token: "",
  isLoggedIn: false,
  onLogin: async (username, password) => {},
  onLogout: () => {},
  getInfo: async () => {},
});

export default AuthContext;
