import React from "react";

const AuthContext = React.createContext({
  token: "",
  isLoggedIn: false,
  id: 0,
  username: "",
  onLogin: async (username, password) => {},
  onLogout: () => {},
});

export default AuthContext;
