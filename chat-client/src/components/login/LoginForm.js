import { Button, Link, TextField, Typography } from "@mui/material";
import { useContext } from "react";
import AuthContext from "../../context/auth-context";
import useInput from "../../hooks/use-input";

const LoginForm = () => {
  const authContext = useContext(AuthContext);
  const usernameInput = useInput((value) => value !== "");
  const passwordInput = useInput((value) => value !== "");

  const resetInputs = () => {
    usernameInput.reset();
    passwordInput.reset();
  };
  const isInputValid = usernameInput.isValid && passwordInput.isValid;
  const handleSubmit = (event) => {
    event.preventDefault();
    if (!isInputValid) {
      return;
    }
    authContext.onLogin(usernameInput.value, passwordInput.value);
    resetInputs();
  };

  return (
    <form onSubmit={handleSubmit}>
      <TextField
        margin="normal"
        id="username"
        label="Username"
        type="text"
        fullWidth
        variant="outlined"
        value={usernameInput.value}
        onChange={usernameInput.valueChangeHandler}
        onBlur={usernameInput.inputBlurHandler}
        error={usernameInput.hasError}
      />
      <TextField
        margin="normal"
        id="password"
        label="Password"
        type="password"
        fullWidth
        variant="outlined"
        value={passwordInput.value}
        onChange={passwordInput.valueChangeHandler}
        onBlur={passwordInput.inputBlurHandler}
        error={passwordInput.hasError}
      />
      <Typography variant="body2">
        Don't have an account? <Link href="/register">Register now.</Link>
      </Typography>
      <Button type="submit" variant="contained" fullWidth sx={{ mt: 1 }}>
        Login
      </Button>
    </form>
  );
};

export default LoginForm;
