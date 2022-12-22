import { Container, Paper } from "@mui/material";
import { useContext, useEffect } from "react";
import { useNavigate } from "react-router-dom";
import RegisterForm from "../components/register/RegisterForm";
import AuthContext from "../context/auth-context";

const Register = () => {
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
        <RegisterForm />
      </Paper>
    </Container>
  );
};

export default Register;
