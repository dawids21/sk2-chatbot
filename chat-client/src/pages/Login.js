import { Container, Paper } from "@mui/material";
import { useContext, useEffect } from "react";
import { useNavigate } from "react-router-dom";
import LoginForm from "../components/login/LoginForm";
import AuthContext from "../context/auth-context";

const Login = () => {
  const { isLoggedIn } = useContext(AuthContext);
  const navigate = useNavigate();
  useEffect(() => {
    if (isLoggedIn) {
      navigate("/");
    }
  }, [isLoggedIn, navigate]);
  return (
    <Container maxWidth="xs">
      <Paper sx={{ p: 2 }}>
        <LoginForm />
      </Paper>
    </Container>
  );
};

export default Login;
