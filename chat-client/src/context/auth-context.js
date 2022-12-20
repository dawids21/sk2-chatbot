import React from "react";

const AuthContext = React.createContext({
  token: '',
  isLoggedIn: false,
  onLogin: (username, password) => { },
  onLogout: () => { }
});

export default AuthContext;
