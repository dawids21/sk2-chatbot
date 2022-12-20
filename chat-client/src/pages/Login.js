import { Button } from "@mui/material";
import { useContext } from "react";
import AuthContext from "../context/auth-context";

const Login = () => {
  const authContext = useContext(AuthContext);
  const handleLogin = () => {
    authContext.onLogin("dawids21", "12345");
  };
  return (
    <>
      <h1>Login</h1>
      <Button onClick={handleLogin}>Login</Button>
    </>
  );
};

export default Login;
